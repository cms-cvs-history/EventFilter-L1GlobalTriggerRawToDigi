import FWCore.ParameterSet.Config as cms

l1GtPack = cms.EDFilter("L1GTDigiToRaw",
    # FED Id for GT DAQ record 
    # default value defined in DataFormats/FEDRawData/src/FEDNumbering.cc
    DaqGtFedId = cms.untracked.int32(813),
    # input tag for GT readout record: 
    #     gtDigis         = GT emulator, 
    #     l1GtUnpack      = GT unpacker 
    DaqGtInputTag = cms.InputTag("gtDigis"),
    # mask for active boards (actually 16 bits)
    #      if bit is zero, the corresponding board will not be packed
    #      default: no board masked
    ActiveBoardsMask = cms.uint32(65535),
    # input tag for GMT readout collection: 
    #     gmtDigis       = GMT emulator, 
    #     l1GtUnpack     = GT unpacker 
    MuGmtInputTag = cms.InputTag("gmtDigis")
)

