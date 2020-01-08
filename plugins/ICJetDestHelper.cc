/* #include "UserCode/ICHiggsTauTau/plugins/ICJetDestHelper.hh" */

/* template<> */
/* std::map<unsigned, unsigned> BuildTrkVtxMap( */
/*     edm::Handle<reco::TrackCollection> const &trk_handle, */
/*     edm::Handle<reco::VertexCollection> const &vtx_handle) { */
/*   std::map<unsigned, unsigned> trk_vtx_map; */
/*   if (trk_handle->size() > 0) { */
/*     reco::Track const *ptr_to_first = &(trk_handle->at(0)); */
/*     for (unsigned i = 0; i < vtx_handle->size(); ++i) { */
/*       std::vector<reco::TrackBaseRef>::const_iterator trk_it; */
/*       for (trk_it = vtx_handle->at(i).tracks_begin(); */
/*            trk_it != vtx_handle->at(i).tracks_end(); ++trk_it) { */
/*         reco::Track const *ptr_to_trk = */
/*             dynamic_cast<reco::Track const *>(&(**trk_it)); */
/*         unsigned trk_idx = (unsigned(ptr_to_trk - ptr_to_first)); */
/*         trk_vtx_map[trk_idx] = i; */
/*       } */
/*     } */
/*   } */
/*   return trk_vtx_map; */
/* } */

/* template<> */
/* void FillPFTracksAndRequest(ic::PFJet *dest, */
/*                             std::vector<reco::PFCandidatePtr> const &pfcands, */
/*                             reco::TrackRefVector *track_requests, */
/*                             boost::hash<reco::Track const *> *track_hasher) { */
/*   std::vector<std::size_t> track_ids; */
/*   for (unsigned iP = 0; iP < pfcands.size(); ++iP) { */
/*     if (pfcands[iP]->trackRef().isNonnull()) { */
/*       track_requests->push_back(pfcands[iP]->trackRef()); */
/*       track_ids.push_back((*track_hasher)(&(*(pfcands[iP]->trackRef())))); */
/*     } */
/*   } */
/*   dest->set_constituent_tracks(track_ids); */
/* } */

/* template<> */
/* void FillJPTTrackVars(ic::JPTJet *dest, */
/*                       std::vector<reco::Track const *> const &all_tracks, */
/*                       edm::Handle<reco::TrackCollection> const &trk_handle, */
/*                       edm::Handle<reco::VertexCollection> const &vtx_handle, */
/*                       std::map<unsigned, unsigned> const &trk_vtx_map) { */
/*   std::vector<double> pt_at_vtx_vec(vtx_handle->size(), 0.0); */
/*   reco::Track const *ptr_first = &(trk_handle->at(0)); */
/*   double trk_pt_total = 0.0; */
/*   for (unsigned j = 0; j < all_tracks.size(); ++j) { */
/*     unsigned idx = unsigned(all_tracks[j] - ptr_first); */
/*     trk_pt_total += all_tracks[j]->pt(); */
/*     // Is track associated to a vertex? */
/*     if (trk_vtx_map.count(idx) > 0) { */
/*       pt_at_vtx_vec[trk_vtx_map.find(idx)->second] += all_tracks[j]->pt(); */
/*     } else {  // No, so is it within 0.2 cm wrt the closest vertex in z */
/*       std::vector<double> dz_with_vtx(vtx_handle->size(), 0.0); */
/*       for (unsigned k = 0; k < vtx_handle->size(); ++k) { */
/*         dz_with_vtx[k] = fabs(vtx_handle->at(k).z() - all_tracks[j]->vz()); */
/*       } */
/*       std::vector<double>::const_iterator min = */
/*           std::min_element(dz_with_vtx.begin(), dz_with_vtx.end()); */
/*       if (min != dz_with_vtx.end()) { */
/*         if (*min < 0.2) { */
/*           pt_at_vtx_vec[unsigned(min - dz_with_vtx.begin())] += */
/*               all_tracks[j]->pt(); */
/*         } */
/*       } */
/*     } */
/*   } */
/*   if (vtx_handle->size() > 0 && trk_pt_total > 0.0) { */
/*     for (unsigned j = 0; j < pt_at_vtx_vec.size(); ++j) { */
/*       pt_at_vtx_vec[j] = pt_at_vtx_vec[j] / trk_pt_total; */
/*     } */
/*     dest->set_beta(pt_at_vtx_vec[0]); */
/*     dest->set_beta_max( */
/*         *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end())); */
/*     dest->set_track_pt_total(trk_pt_total); */
/*   } else { */
/*     dest->set_beta(-1.); */
/*     dest->set_beta_max(-1.); */
/*     dest->set_track_pt_total(0); */
/*   } */
/* } */

/* template<> */
/* void FillPFTrackVars(ic::PFJet *dest, */
/*                      std::vector<reco::PFCandidatePtr> const &pfcands, */
/*                      edm::Handle<reco::TrackCollection> const &trk_handle, */
/*                      edm::Handle<reco::VertexCollection> const &vtx_handle, */
/*                      std::map<unsigned, unsigned> const &trk_vtx_map) { */
/*   int charged_multiplicity_nopu = 0; */
/*   float linear_radial_moment_nopu = 0.0; */
/*   double trk_pt_total = 0.0; */
/*   std::vector<double> pt_at_vtx_vec(vtx_handle->size(), 0.0); */
/*   reco::Track const* ptr_first = &(trk_handle->at(0)); */
/*   for (unsigned iP = 0; iP < pfcands.size(); ++iP) { */
/*     if (pfcands[iP]->trackRef().isNonnull()) { */
/*       unsigned idx = unsigned(&(*(pfcands[iP]->trackRef())) - ptr_first); */
/*       trk_pt_total += pfcands[iP]->trackRef()->pt(); */
/*       // Is track associated to a vertex? */
/*       if (trk_vtx_map.count(idx) > 0) { */
/*         // If this track is mapped to the first PV, increase number of nopu */
/*         // charged multiplicity */
/*         if (trk_vtx_map.find(idx)->second == 0) { */
/*           ++charged_multiplicity_nopu; */
/*           linear_radial_moment_nopu += */
/*               pfcands[iP]->pt() * deltaR(dest->vector().Rapidity(), dest->phi(), */
/*                                          pfcands[iP]->rapidity(), */
/*                                          pfcands[iP]->phi()); */
/*         } */
/*         pt_at_vtx_vec[trk_vtx_map.find(idx)->second] += */
/*             pfcands[iP]->trackRef()->pt(); */
/*       } else {  // No, so is it within 0.2 cm wrt the closest vertex in z */
/*         std::vector<double> dz_with_vtx(vtx_handle->size(), 0.0); */
/*         for (unsigned iV = 0; iV < vtx_handle->size(); ++iV) { */
/*           dz_with_vtx[iV] = */
/*               fabs(vtx_handle->at(iV).z() - pfcands[iP]->trackRef()->vz()); */
/*         } */
/*         std::vector<double>::const_iterator min = */
/*             std::min_element(dz_with_vtx.begin(), dz_with_vtx.end()); */
/*         if (min != dz_with_vtx.end()) { */
/*           if (*min < 0.2) { */
/*             // As above, if this track is not associated to any vertex, but */
/*             // is within 0.2 cm of first PV, then increase multiplicity */
/*             // count */
/*             if (unsigned(min-dz_with_vtx.begin()) == 0) { */
/*               ++charged_multiplicity_nopu; */
/*               linear_radial_moment_nopu += */
/*                   pfcands[iP]->pt() * deltaR(dest->vector().Rapidity(), dest->phi(), */
/*                                              pfcands[iP]->rapidity(), */
/*                                              pfcands[iP]->phi()); */
/*             } */
/*             pt_at_vtx_vec[unsigned(min - dz_with_vtx.begin())] += */
/*                 pfcands[iP]->trackRef()->pt(); */
/*           } */
/*         } */
/*       } */
/*     } else {  // No track found, assume PF candidate is not PU */
/*       linear_radial_moment_nopu += */
/*           pfcands[iP]->pt() * deltaR(dest->vector().Rapidity(), dest->phi(), */
/*                                      pfcands[iP]->rapidity(), */
/*                                      pfcands[iP]->phi()); */
/*     } */
/*   } */
/*   // IMPORTANT: taking the dest pt() here not the src()! */
/*   linear_radial_moment_nopu = linear_radial_moment_nopu / dest->pt(); */
/*   dest->set_linear_radial_moment(linear_radial_moment_nopu); */
/*   dest->set_charged_multiplicity_nopu(charged_multiplicity_nopu); */
/*   // jet.set_constituent_tracks(trks); */
/*   if (vtx_handle->size() > 0 && trk_pt_total > 0.0) { */
/*     for (unsigned iV = 0; iV < pt_at_vtx_vec.size(); ++iV) { */
/*       pt_at_vtx_vec[iV] = pt_at_vtx_vec[iV] / trk_pt_total; */
/*     } */
/*     dest->set_beta(pt_at_vtx_vec[0]); */
/*     dest->set_beta_max( */
/*         *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end())); */
/*   } else { */
/*     dest->set_beta(-1.0); */
/*     dest->set_beta_max(-1.0); */
/*   } */
/* } */

