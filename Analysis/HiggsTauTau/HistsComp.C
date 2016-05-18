{
TString foldername = "l1t-tsg-v5-plots/SignalVsBackground/tautau/";
TString histname = "MttVis";
   
TFile *f1 = new TFile("output/Trigger_2016/DY2JOffline/HTT_11.root");
TH1D * h1 = (TH1D*)f1->Get("OfflineHistogramsPostFiltering/"+histname);

TFile *f2 = new TFile("output/Trigger_2016/DY2JOffline/DY2J_11.root");
TH1D * h2 = (TH1D*)f2->Get("OfflineHistogramsPostFiltering/"+histname);


gStyle->SetOptStat(0);

TGaxis::SetMaxDigits(3);
h1->GetXaxis()->SetTitle("m_{vis} [GeV]");

h2->SetFillColor(kOrange-4);
h1->GetYaxis()->SetTitle("");

h1->SetLineColor(kBlue);
h1->SetLineStyle(9);
h1->SetLineWidth(4);
h2->SetLineColor(kBlack);
h1->SetFillStyle(0);
h1->Rebin(5);
h2->Rebin(5);

double scale1 = scale1 = 10000*3.782 * 0.0627/1497868;
double scale2 = 10000 *332.8 /20019059;

h1->Scale(scale1);
h2->Scale(scale2);

TH1D *h3 = (TH1D*)h1->Clone(0);
for(unsigned i= 1; i<= h1->GetNbinsX(); i++){
    float divideby = sqrt(h1->GetBinContent(i) + h2->GetBinContent(i));
    if(divideby == 0) h3->SetBinContent(i,0);
    else h3->SetBinContent(i,h3->GetBinContent(i)/divideby);    
}
   
float s = h1->Integral(0,h1->GetNbinsX());
h1->Add(h2);

cout << "N_{signal} = " << s << endl;
cout << "N_{background} = " << h2->Integral(0,h2->GetNbinsX()+1) << endl;

TCanvas *c1 = new TCanvas("c1", "canvas", 800, 800);

TLatex *   tex2 = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex2->SetNDC();
tex2->SetTextFont(44);
tex2->SetTextSize(23);
tex2->SetLineWidth(2);
tex2->Draw();
      
//TString seed = "DoubleIsoTau28er";
TString seed = "DiJetAvg60Mjj855";

TLatex *   tex3 = new TLatex(0.15,0.92,seed);
tex3->SetNDC();
tex3->SetTextFont(44);
tex3->SetTextSize(23);
tex3->SetLineWidth(2);
tex3->Draw();

TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 0.9);
pad1->SetTopMargin(0);
pad1->SetBottomMargin(0.2);
pad1->SetFillStyle(0);
pad1->SetBottomMargin(0);
pad1->SetFillColor(kWhite);
pad1->Draw();
pad1->cd(); 

h1->GetYaxis()->SetLabelSize(0.045);
h1->GetXaxis()->SetLabelSize(0.);
h1->GetXaxis()->SetTitle("");
h1->GetYaxis()->SetRangeUser(0.001,1.1*h1->GetMaximum());
h2->GetYaxis()->SetRangeUser(0.001,1.1*h1->GetMaximum());
h1->Draw("");
h2->Draw("same");

pad1->RedrawAxis();


TString NSignal = Form("%.0f",s);   
TString NBack = Form("%.0f",h2->Integral(0,h2->GetNbinsX()+1));

leg = new TLegend(0.62,0.65,0.96,0.83);
leg->SetTextSize(0.05);
leg->SetBorderSize(0);
leg->SetFillStyle(0);
leg->AddEntry((TObject*)0,"","");
leg->AddEntry(h1,"#bf{SM H}#rightarrow#tau#tau ","l");;
leg->AddEntry(h2,"#bf{Z}#rightarrow#tau#tau","f");


leg->Draw();

TLatex *   tex4 = new TLatex(0.57,0.4,"S/B @ 10 fb^{-1} = "+NSignal+"/"+NBack);
tex4->SetNDC();
tex4->SetTextFont(44);
tex4->SetTextSize(23);
tex4->SetLineWidth(2);
tex4->Draw();

TLatex *   tex5 = new TLatex(0.17,0.9,"#tau_{h}#tau_{h}");
tex5->SetNDC();
tex5->SetTextFont(44);
tex5->SetTextSize(30);
tex5->SetLineWidth(2);
tex5->Draw();

c1->cd();  
TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
pad2->SetTopMargin(0);
pad2->SetBottomMargin(0.8);
pad2->SetGridx();
pad2->SetGridy();
pad2->SetFillStyle(0);
pad2->SetTopMargin(0);
pad2->SetBottomMargin(0.2);
pad2->SetFillColor(kWhite);
pad2->Draw();
pad2->cd(); 

h3->GetXaxis()->SetTitleSize (0.1);
h3->GetXaxis()->SetTitleOffset(0.9);
h3->GetXaxis()->SetLabelSize (0.1);
h3->GetYaxis()->SetTitleSize (0.1);
h3->GetYaxis()->SetLabelSize (0.1);
h3->GetYaxis()->SetTitleOffset(0.4);
h3->GetYaxis()->CenterTitle(true);
h3->GetYaxis()->SetTitle("S/#sqrt{S+B}");
h3->GetYaxis()->SetNdivisions(505);
h3->GetYaxis()->SetRangeUser(0,1.3);
h3->SetMarkerStyle(20);
h3->SetMarkerSize(1.5);
h3->SetMarkerColor(kRed);
h3->Draw("p");

   
c1->Print(foldername+seed+".C");
c1->Print(foldername+seed+".png");
c1->Print(foldername+seed+".pdf");

}
