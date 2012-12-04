//Used to study the DCA distributions in the fixed target data.
//Assumes that there is an event tuple and a track tuple

void dcaStudy(TString INPUTFILE){

  //Set Pion Multiplicity Levels
  Float_t minPionMult = 5.0;
  Float_t top10PionMult = 11.0;


  //Open ROOT File for reading and define TTrees
  TFile *inputFile = new TFile(INPUTFILE);
  TTree *eventTree = (TTree*)inputFile->Get("mEventInfo");
  TTree *trackTree = (TTree*)inputFile->Get("mTrackInfo");

  //Define the DCA Histograms to be filled
  TH1F *dcaPionMult_0_10 = new TH1F("dcaTop10","dcaTop10",100,-10,10);
  TH1F *dcaPionMult_10_20 = new TH1F("dcaTop_10_20","dcaTop_10_20",100,-10,10);

  //Define Event Variables
  Float_t eventNumber=-1, eventRunNumber=-1;
  Float_t nEventTracks, chargedMult, pionMult;

  //Define Track Variables
  Float_t pZ, pT, charge, trackRunNumber=-10, trackEventNumber=-10;
  Float_t dcaD;
  Float_t trackIndex=0;

  //Additional Variables
  Float_t Efficiency, pionRapidity,transMomentum;
  
  //Set Branch Addresses
  eventTree->SetBranchAddress("eventNum",&eventNumber);
  eventTree->SetBranchAddress("runNum",&eventRunNumber);
  eventTree->SetBranchAddress("nPi",&pionMult);

  trackTree->SetBranchAddress("pz",&pZ);
  trackTree->SetBranchAddress("pt",&pT);
  trackTree->SetBranchAddress("charge",&charge);
  trackTree->SetBranchAddress("dcaD",&dcaD);
  trackTree->SetBranchAddress("runNum",&trackRunNumber);
  trackTree->SetBranchAddress("eventNum",&trackEventNumber);

  //Get Number of Entries in Tree
  Double_t nEvents = (Double_t)eventTree->GetEntries();
  Double_t nTracks = (Double_t)trackTree->GetEntries();

  //Loop Over Events
  for (Double_t i=0; i<nEvents; i++){
    eventTree->GetEntry(i);
    
    //Skip events with insufficienct pion Multiplicity
    // if (pionMult < minPionMult)
    // continue;

    //cout <<"made it to here" <<endl;

    //Loop Over Tracks
    for (Double_t j=trackIndex; j<nTracks; j++){
      
      //Get the Track's information
      trackTree->GetEntry(j);

	//Print for Testing
	if(0){
	  cout <<"Event Number: " <<eventNumber <<" "
	       <<"Track Event Number: " <<trackEventNumber <<" "
	       <<"Event Run Number: " <<eventRunNumber <<" "
	       <<"Track Run Number: " <<trackRunNumber <<" "
	       <<"Track Index: " <<j <<" "
	       <<"Pion Multiplicity: " <<pionMult <<" "
	       <<endl;
	}

      //If the track's event number  and run Number matches the event's event 
      //number and run number add it to the event statistics
      if (trackEventNumber==eventNumber && trackRunNumber==eventRunNumber){
	nEventTracks++;

	//Fill the Correct DCA Histogram depending on Pion Multiplicity
	if (pionMult > top10PionMult){
	  dcaPionMult_0_10->Fill(dcaD);
	}
	if(pionMult < top10PionMult)
	  dcaPionMult_10_20->Fill(dcaD);
      }

      //If the track's event number and run number don't match the event's 
      //event number and run number move onto the next track
      if (trackEventNumber!=eventNumber || trackRunNumber!=eventRunNumber){
	trackIndex=j;
	break;
      }

      //Print Progress
      cout <<"Percent Complete: "
	   <<(i/nEvents)*100 <<" \r";



    }//End Track Loop

  }//End Event Loop

  //Create First Canvas
  TCanvas *c1 = new TCanvas();
 
  //Draw Histogram
  dcaPionMult_0_10->Draw();

  //Create Second Canvas
  TCanvas *c2 = new TCanvas();
  dcaPionMult_10_20->Draw();

}// End dcaStudy
