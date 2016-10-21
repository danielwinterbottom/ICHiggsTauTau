{
std::vector<std::string> channels= {"em","et","mt","tt"};
std::vector<std::string> sig = {"gluglu", "vbf"};

TH1D *h1 = new TH1D("h1", "",31,0,31);
TH1D *h2 = new TH1D("h2", "",31,0,31);
TH1D *h3 = new TH1D("h3", "",31,0,31);
TH1D *h4 = new TH1D("h4", "",31,0,31);
TH1D *h5 = new TH1D("h5", "",31,0,31);
TH1D *h6 = new TH1D("h6", "",31,0,31);
TH1D *h7 = new TH1D("h7", "",31,0,31);
TH1D *h8 = new TH1D("h8", "",31,0,31);

gStyle->SetPadBottomMargin(0.25);
gStyle->SetPadGridX(1);
gStyle->SetPadGridY(1);
for(unsigned i=0; i< sig.size(); ++i){
  int count = 1;
  for(unsigned j=0; j< channels.size(); ++j){
    std::string file_name = "theory_output_"+sig[i]+"_"+channels[j]+"/uncertainties.txt";
    
    ifstream infile (file_name);
    if(infile.is_open()){
      std::string cat;
      double scale;
      double nnpdf;
      double pdf;
      double alpha_s;
      while ( infile >> cat >> scale >> nnpdf >> pdf >> alpha_s)
      {
        std::string label = channels[j]+"_"+cat;
        if(sig[i] == "gluglu"){
          h1->SetBinContent(count,scale);
          h1->GetXaxis()->SetBinLabel(count,label.c_str());
          h1->GetXaxis()->CenterLabels(); 
          h1->GetXaxis()->LabelsOption("v");
          h1->SetLineStyle(0);
          h1->SetMarkerStyle(20);
          h1->SetMarkerColor(4);
          h1->GetYaxis()->SetTitle("Scale uncertainty");
          h1->GetYaxis()->SetTitleOffset(1.6);
          
          h2->SetBinContent(count,nnpdf);
          h2->GetXaxis()->SetBinLabel(count,label.c_str());
          h2->GetXaxis()->CenterLabels(); 
          h2->GetXaxis()->LabelsOption("v");
          h2->SetLineStyle(0);
          h2->SetMarkerStyle(20);
          h2->SetMarkerColor(4);
          h2->GetYaxis()->SetTitle("PDF uncertainty [NNPDF only]");
          h2->GetYaxis()->SetTitleOffset(1.6);
          
          h3->SetBinContent(count,pdf);
          h3->GetXaxis()->SetBinLabel(count,label.c_str());
          h3->GetXaxis()->CenterLabels(); 
          h3->GetXaxis()->LabelsOption("v");
          h3->SetLineStyle(0);
          h3->SetMarkerStyle(20);
          h3->SetMarkerColor(4);
          h3->GetYaxis()->SetTitle("PDF uncertainty [NNPDF + CT10 + MMHT]");
          h3->GetYaxis()->SetTitleOffset(1.6);
          
          h4->SetBinContent(count,alpha_s);
          h4->GetXaxis()->SetBinLabel(count,label.c_str());
          h4->GetXaxis()->CenterLabels(); 
          h4->GetXaxis()->LabelsOption("v");
          h4->SetLineStyle(0);
          h4->SetMarkerStyle(20);
          h4->SetMarkerColor(4);
          h4->GetYaxis()->SetTitle("#alpha_{s} uncertainty");
          h4->GetYaxis()->SetTitleOffset(1.6);
          
          count++;
        } else if (sig[i] == "vbf"){
          h5->SetBinContent(count,scale);
          h5->GetXaxis()->SetBinLabel(count,label.c_str());
          h5->GetXaxis()->CenterLabels(); 
          h5->GetXaxis()->LabelsOption("v");
          h5->SetLineStyle(0);
          h5->SetMarkerStyle(20);
          h5->SetMarkerColor(4);
          h5->GetYaxis()->SetTitle("Scale uncertainty");
          h5->GetYaxis()->SetTitleOffset(1.6);
          
          h6->SetBinContent(count,nnpdf);
          h6->GetXaxis()->SetBinLabel(count,label.c_str());
          h6->GetXaxis()->CenterLabels(); 
          h6->GetXaxis()->LabelsOption("v");
          h6->SetLineStyle(0);
          h6->SetMarkerStyle(20);
          h6->SetMarkerColor(4);
          h6->GetYaxis()->SetTitle("PDF uncertainty [NNPDF only]");
          h6->GetYaxis()->SetTitleOffset(1.6);
          
          h7->SetBinContent(count,pdf);
          h7->GetXaxis()->SetBinLabel(count,label.c_str());
          h7->GetXaxis()->CenterLabels(); 
          h7->GetXaxis()->LabelsOption("v");
          h7->SetLineStyle(0);
          h7->SetMarkerStyle(20);
          h7->SetMarkerColor(4);
          h7->GetYaxis()->SetTitle("PDF uncertainty [NNPDF + CT10 + MMHT]");
          h7->GetYaxis()->SetTitleOffset(1.6);
          
          h8->SetBinContent(count,alpha_s);
          h8->GetXaxis()->SetBinLabel(count,label.c_str());
          h8->GetXaxis()->CenterLabels(); 
          h8->GetXaxis()->LabelsOption("v");
          h8->SetLineStyle(0);
          h8->SetMarkerStyle(20);
          h8->SetMarkerColor(4);
          h8->GetYaxis()->SetTitle("#alpha_{s} uncertainty");
          h8->GetYaxis()->SetTitleOffset(1.6);
          
          count++;
        }
      }
      infile.close();
    }
  }
  std::cout << count << std::endl;
}

TLatex *tex1 = new TLatex(0.8,0.92,"ggH");
tex1->SetNDC();
tex1->SetTextFont(44);
tex1->SetTextSize(23);
tex1->SetLineWidth(2);

TLatex *tex2 = new TLatex(0.8,0.92,"qqH");
tex2->SetNDC();
tex2->SetTextFont(44);
tex2->SetTextSize(23);
tex2->SetLineWidth(2);

h1->Draw("p");
tex1->Draw();
c1->Print("scale_ggH.pdf");

h2->Draw("p");
tex1->Draw();
c1->Print("nnpdf_ggH.pdf");

h3->Draw("p");
tex1->Draw();
c1->Print("pdf_ggH.pdf");

h4->Draw("p");
tex1->Draw();
c1->Print("alpha_s_ggH.pdf");

h5->Draw("p");
tex2->Draw();
c1->Print("scale_qqH.pdf");

h6->Draw("p");
tex2->Draw();
c1->Print("nnpdf_qqH.pdf");

h7->Draw("p");
tex2->Draw();
c1->Print("pdf_qqH.pdf");

h8->Draw("p");
tex2->Draw();
c1->Print("alpha_s_qqH.pdf");
}
