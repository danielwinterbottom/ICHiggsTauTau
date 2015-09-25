#!/usr/bin/env python
import requests
import json
from optparse import OptionParser, OptionGroup
import subprocess
import os
import sys
import hmac
from hashlib import sha1
from flask import Flask, request, abort
import datetime
import time
import calendar
requests.packages.urllib3.disable_warnings()

parser = OptionParser()
parser.add_option('-m', '--mode',
                  type = 'choice',
                  dest = 'mode',
                  choices = ['single', 'listen', 'poll'],
                  default = 'poll',
                  help = ('single: test a particular PR,'
                          'listen: start server to listen for comment requests via pushed webhooks,'
                          'poll: check github periodically for comment requests'))
parser.add_option('--gh-user',
                  dest='gh_user',
                  help='Run as named GitHub user')
parser.add_option('--gh-token',
                  dest='gh_token',
                  default = '',
                  help='github authentication token')
parser.add_option('-c', '--config',
                  dest='config',
                  help='configuration JSON file')
parser.add_option('-u', '--upload',
                  dest = 'upload',
                  action = 'store_true',
                  default = False,
                  help='Upload log files to gist and update PR status')
parser.add_option('--gist',
                  dest = 'gist',
                  default = '',
                  help='The id of the github gist where the logs should be uploaded')
parser.add_option('--trigger',
                  dest = 'trigger',
                  default = 'please test',
                  help='Comment phrase to trigger a build')

grp_single = OptionGroup(parser, 'single mode options')
grp_single.add_option('--pr',
                  dest = 'pr',
                  type = 'int',
                  default = None,
                  help = 'Pull request number to test')
grp_single.add_option('--clean',
                  dest = 'clean',
                  action = 'store_true',
                  default = False,
                  help = 'Call the clean script first')
parser.add_option_group(grp_single)

grp_listen = OptionGroup(parser, 'listen mode options')
grp_listen.add_option('--port',
                  dest = 'port',
                  type = 'int',
                  default = 5000,
                  help='Listen for requests on this port')
grp_listen.add_option('--key',
                  dest = 'hook_key',
                  default = None,
                  help='Check incoming hooks are signed with this key')
parser.add_option_group(grp_listen)

grp_poll = OptionGroup(parser, 'poll mode options')
grp_poll.add_option('--interval',
                  dest = 'interval',
                  type = 'float',
                  default = 600.,
                  help = 'Interval for github polling in seconds')
parser.add_option_group(grp_poll)

(opts, args) = parser.parse_args()

# Check input makes sense
if not opts.gh_user:
    raise RuntimeError('No github user specified')
if not opts.gh_token:
    raise RuntimeError('No github token specified')
if not opts.config:
    raise RuntimeError('No config file specified')

if opts.mode == 'single':
    if not opts.pr:
        raise RuntimeError('Must specify a PR in single mode')

# Don't let the user do something stupid like put 0.001 seconds
if opts.mode == 'poll' and opts.interval < 300.:
    raise RuntimeError('Minimum polling interval is 300 seconds')

# Add some extra stuff
if opts.mode == 'listen' and not opts.hook_key:
    raise RuntimeError('A hook key is required')

if opts.upload and not opts.gist:
    raise RuntimeError('Upload requested but no gist specified')

if type(opts.hook_key) == unicode:
    opts.hook_key = opts.hook_key.encode()
opts.gh_token           = opts.gh_token.strip()
opts.gist               = opts.gist.strip()
opts.config             = json.load(open(opts.config))
opts.api                = 'https://api.github.com'
opts.repo               = 'ajgilbert/ICHiggsTauTau'
opts.collaborators_url  = opts.api + '/repos/' + opts.repo + '/collaborators'
opts.time_fmt           = '%Y-%m-%dT%H:%M:%SZ'

# Have to declare our flask app here
app = Flask(__name__)
app.debug = False

def DumpJson(r):
  return json.dumps(r, sort_keys=False, indent=4, separators=(',', ': '))

def compare_digest(a, b):
    if len(a) != len(b):
        return False
    result = 0
    for ch_a, ch_b in zip(a, b):
        result |= ord(ch_a) ^ ord(ch_b)
    return result == 0

@app.route("/", methods=['POST'])
def index():
    # First check that signature on this POST request is valid
    signature = request.headers.get('X-Hub-Signature').split('=')[1]
    mac = hmac.new(opts.hook_key, msg=request.data, digestmod=sha1)
    if not compare_digest(mac.hexdigest(), signature):
        abort(403)
    # We are only interested in "issue_comment" events
    if request.headers.get('X-GitHub-Event') != "issue_comment":
        return 'OK'
    # Now we need to dig into the json data
    payload = json.loads(request.data)
    # Only want comments on PRs, not generic issues
    if 'pull_request' not in payload['issue']:
        return 'OK'
    # Does this message contain the trigger phrase?
    if payload['comment']['body'].lower() != opts.trigger.lower():
        return 'OK'
    # We will only launch the build if the requesting user is an admin on the repo
    pr = payload['issue']['number']
    user = payload['comment']['user']['login']
    print '>> Received a build trigger comment on PR #' + str(pr) + ' from user ' + user
    chk_req = requests.get(opts.collaborators_url + '/' + user, auth=(opts.gh_user, opts.gh_token))
    if chk_req.status_code == 204:
        print '>> This user is a collaborator on the repo so we are safe to proceed'
    else:
        print '>> User not a collaborator on the repo - aborting'
        abort(403)
    runBuild(opts.gh_user, opts.gh_token, pr, opts.config, user)
    return 'OK'

def runBuild(gh_user, gh_token, pr, config, req_user, do_clean=False):
  print '>> Build request running as user ' + gh_user

  # Query the github api for info on this PR
  auth = (gh_user, gh_token)
  pr_req = requests.get('%s/repos/%s/pulls/%i' % (opts.api, opts.repo, pr), auth=auth)
  if not pr_req.status_code == 200:
    print '>> Something went wrong with the github API request, aborting'
    print DumpJson(pr_req.json())
    return

  # Extract some useful info from the headers & json data
  thedate           = pr_req.headers['Date']
  pr_json           = pr_req.json()
  pr_is_mergeable   = pr_json['mergeable']
  pr_title          = pr_json['title']
  pr_is_open        = pr_json['state'] == 'open'
  statuses_url      = pr_json["statuses_url"]

  if not pr_is_open:
      print '>> PR is not open, aborting build'
      return
  if not pr_is_mergeable:
      print '>> PR is not mergeable, aborting build'
      return

  print '>> Will test PR #%i: %s' % (pr, pr_title)

  status = {}

  # Set the status for each job to 'pending' if we're uploading
  if opts.upload:
    for task in config.keys():
      payload = {
        'state'       : 'pending',
        'target_url'  : '',
        'description' : 'Started by %s on %s' % (req_user, thedate),
        'context'     : task
      }
      status_resp = requests.post(statuses_url, auth=auth, data=json.dumps(payload))

  # Run the actual jobs
  for task in config.keys():
    params = config[task]
    print ">>>> Running task: " + task
    outlogf = open('%s.log' % task, 'w')
    if do_clean:
      command = './run_builds.sh %s %s %s %s %s %s %s' % (
        params["build_root"], params["cmssw_env_script"],
        params["cmssw_version"], params["cmssw_user"],
        params["cmssw_branch"], pr, params["clean_script"])
      print '>>>> Command: ' + command
      job = subprocess.Popen(command, shell=True, stdout=outlogf, stderr=subprocess.STDOUT)
      job.communicate()
    command = './run_builds.sh %s %s %s %s %s %s %s' % (
      params["build_root"], params["cmssw_env_script"],
      params["cmssw_version"], params["cmssw_user"],
      params["cmssw_branch"], pr, params["test_script"])
    print '>>>> Command: ' + command
    job = subprocess.Popen(command, shell=True, stdout=outlogf, stderr=subprocess.STDOUT)
    job.communicate()
    status[task] = job.returncode

  print 'Task Results'
  print '------------'
  for (task, st) in status.iteritems():
    state = 'success' if st == 0 else 'failure'
    print '%-40s %s' % (task, 'SUCESS' if st == 0 else 'FAILED')
    # First upload the log to gist
    if opts.upload:
      outlog = '%s.log' % task
      gist_payload = {
        'description': 'Validation log files',
        'files': {
          outlog: { 'content': open(outlog, 'r').read() }
        }
      }
      gist_resp = requests.patch('%s/gists/%s' % (opts.api, opts.gist), auth=auth, data=json.dumps(gist_payload))
      raw_url = gist_resp.json()['files'][outlog]['raw_url']
      payload = {
        'state'       : state,
        'target_url'  : raw_url,
        'description' : 'Task started by %s on %s' % (req_user, thedate),
        'context'     : task
      }
      status_resp = requests.post(statuses_url, auth=auth, data=json.dumps(payload))

# Check if there is a valid comment
def pollGitHub(gh_user, gh_token, begin, end):
  # We have to convert epoch time -> UTC time struct -> UTC string
  since_stamp = time.strftime(opts.time_fmt, time.gmtime(begin))
  auth = (gh_user, gh_token)
  comments_json = requests.get('%s/repos/%s/issues/comments?since=%s' % (opts.api, opts.repo, since_stamp), auth=auth).json()
  ret = set()
  all_prs = set()
  trg = opts.trigger.lower().strip()
  trg_clean = trg + ' clean'
  for com in comments_json:
      clean = False
      comment_time = calendar.timegm(time.strptime(com['created_at'], opts.time_fmt))
      # Comment must have occured in interval [begin, end)
      if comment_time < begin or comment_time >= end:
          continue
      comment = com['body'].lower().strip()
      if comment == trg_clean:
        clean = True
      if comment not in [trg, trg_clean]:
          continue
      issue_json = requests.get(com['issue_url'], auth=auth).json()
      is_pr = 'pull_request' in issue_json
      if not is_pr:
          continue
      pr = issue_json['number']
      user = com['user']['login']
      print '>> Received a build request on PR #' + str(pr) + ' from user ' + user
      chk_req = requests.get(opts.collaborators_url+'/'+user, auth=auth)
      if chk_req.status_code == 204:
          print '>> This user is a collaborator on the repo so we are safe to proceed'
      else:
          continue
      if pr not in all_prs: ret.add((pr, user, clean))
      all_prs.add(pr)
  return ret


if __name__ == "__main__":
    if opts.mode == 'listen':
        app.run(host='0.0.0.0', port=opts.port)
    if opts.mode == 'single':
        runBuild(opts.gh_user, opts.gh_token, opts.pr, opts.config, opts.gh_user, opts.clean)
    if opts.mode == 'poll':
        # Initial time frame will be [now, now+interval]
        interval  = opts.interval
        t_start   = time.time()
        t_end     = t_start + interval
        while True:
            # print 'Next interval to run is: '
            # print time.strftime('%Y-%m-%dT%H:%M:%S', time.gmtime(t_start)) + " -> " + time.strftime('%Y-%m-%dT%H:%M:%S', time.gmtime(t_end))
            # is t_end still in the future?
            # If so we'll sleep until ready then check again
            time_now = time.time()
            if t_end > time_now:
                sleep_time = t_end - time_now
                # print 'Need to wait for ' + str(sleep_time) + ' seconds...'
                time.sleep(sleep_time + 0.5)
            else:
                # We've passed the end of our time frame, so poll github
                # print 'OK - polling now...'
                pr_set = pollGitHub(opts.gh_user, opts.gh_token, t_start, t_end)
                # See if we have any builds to do
                for pr in pr_set:
                  runBuild(opts.gh_user, opts.gh_token, pr[0], opts.config, pr[1], pr[2])
                # Create a new time frame starting from the end of the previous one
                t_start = t_end
                t_end   = t_start + interval
                # However, if we've already passed the new t_end (probably
                # because the jobs took a long time), expand t_end to the
                # current time
                time_now = time.time()
                if time_now > t_end:
                    t_end = time_now



