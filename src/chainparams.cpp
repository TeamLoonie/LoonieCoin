// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The Pivx developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of(0, uint256("0x01"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1540235479, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1540283195,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1540283195,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7d;
        pchMessageStart[1] = 0xe1;
        pchMessageStart[2] = 0xa0;
        pchMessageStart[3] = 0x6b;
        vAlertPubKey = ParseHex("04737fc14103be1722ab8f81fd63b764efd255aaec57e960e48d99682e46ed99188e995d75361301e76e8e2dd3bcec3904219ebe13e14862059cc718784624d21b");
        nDefaultPort = 3734;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 60;
        nTargetSpacing = 60;
        nLastPOWBlock = 500;
        nMaturity = 100;
        nModifierUpdateBlock = 0; // always use stake modifier v2
        nMaxMoneyOut = 130000000 * COIN;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Shanghai Composite: surges on hopes of market support, 22 October, 2018";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 250 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04df5cc8133f524f2a70a5902db5bc80b9496a6919b02e4163deb1b10a17e6c81c96b1f77d5275a0dd29a3a072859c6f5327b316c1383b54dc3fbb7a075e28e430") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1540235479;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 2672491;

//                         if(genesis.GetHash() != uint256("0x"))
// {
//       printf("MSearching for genesis block...\n");
//       uint256 hashTarget;
//       hashTarget.SetCompact(genesis.nBits);
//       while(uint256(genesis.GetHash()) > uint256(hashTarget))
//       {
//           ++genesis.nNonce;
//           if (genesis.nNonce == 0)
//           {
//               printf("Mainnet NONCE WRAPPED, incrementing time");
//               std::cout << std::string("Mainnet NONCE WRAPPED, incrementing time:\n");
//               ++genesis.nTime;
//           }
//           if (genesis.nNonce % 10000 == 0)
//           {
//               printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
//           }
//       }
//       printf("Mainnet block.nTime = %u \n", genesis.nTime);
//       printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
//       printf("Mainnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
//       printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
// }

        hashGenesisBlock = genesis.GetHash();




        assert(hashGenesisBlock == uint256("0x00000722ffbc7cb691f8a07ea08bf54ef6fac6d9f4719f3d1bc369a7ff7cab8b"));
        assert(genesis.hashMerkleRoot == uint256("0x31e192827712a2194171d0718104721c9c98c526bb8001f27d008f16ecfd7f70"));
        vFixedSeeds.clear();
        vSeeds.clear();

        
        // vSeeds.push_back(CDNSSeedData("Seed node", "192.64.116.65"));     // Seed node
        // vSeeds.push_back(CDNSSeedData("Seed node", "142.93.168.42"));     // Seed node

        // LNI coin addresses start with 'L'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48);
        // LNI coin script addresses start with '3'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 2);
        // LNI coin private keys start with 'B'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x16)(0x9b)(0x83)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x0b)(0xcc)(0xe2)(0x2b).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x5d).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04c03a20082e774c6c93da0251e00ed00e73b28d2f3bbb692b7ab7ec64a8a7b9374c16afa15b6d7355fc482b5aa6b8fc7ec4d0fb71fcc21bdd9b07bf06a7c9d611";
        //strSporkKey = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
        strPurificationPoolDummyAddress = "D87q2gC9t6HtSCzCsg4aUg8bg1sT9nUhEw";
        nStartMasternodePayments = genesis.nTime + 21600; //24 hrs after genesis
        //nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xbb;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0x39;
        pchMessageStart[3] = 0x88;
        vAlertPubKey = ParseHex("041b2b4c86273359acac3522471911ed2b303eaab65e8a1de01c06e89f2eab1e55234a4b504f3ce20c6f661f007d0ca15623b4358d9855c7c8ba793a24fa315e22");
        nDefaultPort = 14315;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nMaturity = 15;
        nLastPOWBlock = 43800;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1540283195;
        genesis.nNonce = 3132613;

//                         if(genesis.GetHash() != uint256("0x"))
// {
//       printf("MSearching for genesis block...\n");
//       uint256 hashTarget;
//       hashTarget.SetCompact(genesis.nBits);
//       while(uint256(genesis.GetHash()) > uint256(hashTarget))
//       {
//           ++genesis.nNonce;
//           if (genesis.nNonce == 0)
//           {
//               printf("Maintest NONCE WRAPPED, incrementing time");
//               std::cout << std::string("Maintest NONCE WRAPPED, incrementing time:\n");
//               ++genesis.nTime;
//           }
//           if (genesis.nNonce % 10000 == 0)
//           {
//               printf("Maintest: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
//           }
//       }
//       printf("testnet block.nTime = %u \n", genesis.nTime);
//       printf("testnet block.nNonce = %u \n", genesis.nNonce);
//       printf("testnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
//       printf("testnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
// }
        
        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000008c25162f6a432c4a075a1b3ba508227134da3d09337a8714e3d3710ee0e"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // vSeeds.push_back(CDNSSeedData("loonie.io", "seeds-testnet.loonie.io"));
        // vSeeds.push_back(CDNSSeedData("happycat.gq", "tloonie.happycat.gq"));

         base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48); // Testnet skx addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 2);  // Testnet skx script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x22)(0x6a)(0xca)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x22)(0x6a)(0x88)(0x4d).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04abb5e65280dda6a113fadfb9877f9c399532245fe1acb61de293ab298034d5084277fab3768774a3b68cbbe5021cc5049ec8c9997a13f64da1afa0bcfb156db1";
        strPurificationPoolDummyAddress = "G57cqhGRk6kRfDRNeJiL4YVEbFhXrNbKox";
        nStartMasternodePayments = genesis.nTime; //Fri, 09 Jan 2015 21:05:58 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xd6;
        pchMessageStart[2] = 0x33;
        pchMessageStart[3] = 0x3c;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // LOONIE: 1 day
        nTargetSpacing = 1 * 60;        // LOONIE: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1540284178;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12345;

//                         if(genesis.GetHash() != uint256("0x"))
// {
//       printf("MSearching for genesis block...\n");
//       uint256 hashTarget;
//       hashTarget.SetCompact(genesis.nBits);
//       while(uint256(genesis.GetHash()) > uint256(hashTarget))
//       {
//           ++genesis.nNonce;
//           if (genesis.nNonce == 0)
//           {
//               printf("REGnet NONCE WRAPPED, incrementing time");
//               std::cout << std::string("REGnet NONCE WRAPPED, incrementing time:\n");
//               ++genesis.nTime;
//           }
//           if (genesis.nNonce % 10000 == 0)
//           {
//               printf("REGnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
//           }
//       }
//       printf("reg block.nTime = %u \n", genesis.nTime);
//       printf("reg block.nNonce = %u \n", genesis.nNonce);
//       printf("reg block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
//       printf("reg block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
// }

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;
        assert(hashGenesisBlock == uint256("0x75e2274ba3fc7441b769ed6a9d15dd4703af4a43d1f65e3f19b94e3d0d93f085"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
