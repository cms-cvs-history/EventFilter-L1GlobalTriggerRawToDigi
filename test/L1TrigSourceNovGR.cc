// -*- C++ -*-
//
// Package:    L1TrigSourceNovGR
// Class:      L1TrigSourceNovGR
// 
/**\class L1TrigSourceNovGR L1TrigSourceNovGR.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ivan Mikulec
//         Created:  Tue Dec 11 15:54:04 CET 2007
// $Id$
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"
//
// class decleration
//

class L1TrigSourceNovGR : public edm::EDAnalyzer {
   public:
      explicit L1TrigSourceNovGR(const edm::ParameterSet&);
      ~L1TrigSourceNovGR();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1TrigSourceNovGR::L1TrigSourceNovGR(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


L1TrigSourceNovGR::~L1TrigSourceNovGR()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1TrigSourceNovGR::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
   iEvent.getByLabel("l1GtUnpack",gmtrc_handle);
   L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();
   
   bool dt_l1a = false;
   bool rpc_l1a = false;
   bool hcal_l1a = false;
   
   std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
   std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
   
   for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {

     std::vector<L1MuRegionalCand>::const_iterator iter1;
     std::vector<L1MuRegionalCand> rmc;

     int idt = 0;
     rmc = igmtrr->getDTBXCands();
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if ( !(*iter1).empty() ) {
            idt++;
       }
     }
     
     if(idt>0) std::cout << "Found " << idt << " valid DT candidates in bx wrt. L1A = " 
                                     << igmtrr->getBxInEvent() << std::endl;
     if(igmtrr->getBxInEvent()==0 && idt>0) dt_l1a = true;
     
     int irpcb = 0;
     rmc = igmtrr->getBrlRPCCands();
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if ( !(*iter1).empty() ) {
            irpcb++;
       }
     }
     
     if(irpcb>0) std::cout << "Found " << irpcb << " valid RPC candidates in bx wrt. L1A = " 
                                     << igmtrr->getBxInEvent() << std::endl;
     if(igmtrr->getBxInEvent()==0 && irpcb>0) rpc_l1a = true;
     
   }
   
   edm::Handle<L1GlobalTriggerReadoutRecord> gtrr_handle; 
   iEvent.getByLabel("l1GtUnpack",gtrr_handle);
   L1GlobalTriggerReadoutRecord const* gtrr = gtrr_handle.product();
   
   for(int ibx=-1; ibx<=1; ibx++) {
     bool hcal_top = false;
     bool hcal_bot = false;
     const L1GtPsbWord psb = gtrr->gtPsbWord(0xbb0d,ibx);
     std::vector<int> valid_phi;
     if((psb.aData(4)&0x3f) > 1) {valid_phi.push_back( (psb.aData(4)>>10)&0x1f ); }
     if((psb.bData(4)&0x3f) > 1) {valid_phi.push_back( (psb.bData(4)>>10)&0x1f ); }
     if((psb.aData(5)&0x3f) > 1) {valid_phi.push_back( (psb.aData(5)>>10)&0x1f ); }
     if((psb.bData(5)&0x3f) > 1) {valid_phi.push_back( (psb.bData(5)>>10)&0x1f ); }
     std::vector<int>::const_iterator iphi;
     for(iphi=valid_phi.begin(); iphi!=valid_phi.end(); iphi++) {
       std::cout << "Found HCAL mip with phi=" << *iphi << " in bx wrt. L1A = " << ibx << std::endl;
       if(*iphi<9) hcal_top=true;
       if(*iphi>8) hcal_bot=true;
     }
     if(ibx==0 && hcal_top && hcal_bot) hcal_l1a=true;
   }
   
   std::cout << "**** Trigger Source ****" << std::endl;
   if(dt_l1a) std::cout << "DT" << std::endl;
   if(rpc_l1a) std::cout << "RPC" << std::endl;
   if(hcal_l1a) std::cout << "HCAL" << std::endl;
   std::cout << "************************" << std::endl;
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
L1TrigSourceNovGR::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TrigSourceNovGR::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TrigSourceNovGR);
