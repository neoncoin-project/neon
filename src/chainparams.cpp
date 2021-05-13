// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <arith_uint256.h>
#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    txNew.nTime = nTimeTx;

    CBlock genesis;
    genesis.nTime    = nTimeBlock;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "test genesis until release N1";
    const CScript genesisOutputScript = CScript();
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        //consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.powLimit =            uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 32;
        consensus.bnInitialHashTarget = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 40;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 10 * 60; // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24 * 30; // minimum age for coin age
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 6 * 60 * 60; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 500;

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xa3a0ffa0dbca75923ad6a53d3878d62f8b35c363282df3f13ded9e4fda921e63");  // 380000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */

        //neon: new random pch
        pchMessageStart[0] = 0x52;
        pchMessageStart[1] = 0x12;
        pchMessageStart[2] = 0xa1;
        pchMessageStart[3] = 0x59;

        nDefaultPort = 9901; //P2P port
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1620820699, 1620820699, 3501758531u, 0x1d00ffff, 1, 0);

#ifdef GENESIS
        arith_uint256 target;
        target.SetCompact(genesis.nBits);
        while (UintToArith256(genesis.GetHash()) > target) {
            if (genesis.nNonce % 1048576 == 0)
                printf("nonce=%dM hash=%s\n", genesis.nNonce / 1048576, genesis.GetHash().ToString().c_str());
            genesis.nNonce++;
        }

        printf("hashGenesisBlock=%s\n", genesis.GetHash().ToString().c_str());
        printf("hashMerkleRoot=%s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("genesis.nNonce=%d\n", genesis.nNonce);
#endif

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000c5b61b8492d718850487238cded2f046efb0557bf3c260822741dfb8"));
        assert(genesis.hashMerkleRoot == uint256S("0x4fda437eb0fbdfe863d1b755074266c54a004b4136505f3e0e29eb70436b6342"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.

        //vSeeds.emplace_back("goodneonnode.com"); //duplicate this line with more neon nodes

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,20);  // neon: addresses begin with '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,117);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,183);
        //neon: new random keys
        base58Prefixes[EXT_PUBLIC_KEY] = {0x40, 0x43, 0x58, 0x31};
        base58Prefixes[EXT_SECRET_KEY] = {0x40, 0xc3, 0x6f, 0xff};

        // human readable prefix to bench32 address
        bech32_hrp = "neon";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {     0, uint256S("0x00000000c5b61b8492d718850487238cded2f046efb0557bf3c260822741dfb8")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block GEN_HASH (height 0).
            1620820699, // * UNIX timestamp of last known number of transactions
            0,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the ChainStateFlushed debug.log lines)
            0.001       // * estimated number of transactions per second after that timestamp
                        // 1797921/(1591618067-1345400356) = 0.00730216
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        //consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.powLimit =            uint256S("0000000fffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 28;
        consensus.bnInitialHashTarget = uint256S("00000007ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 29;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 10 * 60;  // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000002e9e7b00e1f6dc5123a04aad68dd0f0968d8c7aa45f6640795c37b1"); //1135275

        //neon: new random pch
        pchMessageStart[0] = 0x32;
        pchMessageStart[1] = 0x59;
        pchMessageStart[2] = 0x95;
        pchMessageStart[3] = 0x8d;

        nDefaultPort = 8803;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 2;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1620866102, 1620866102, 4858560u, 0x1d0fffff, 1, 0);

#ifdef GENESIS
        arith_uint256 target;
        target.SetCompact(genesis.nBits);
        while (UintToArith256(genesis.GetHash()) > target) {
            if (genesis.nNonce % 1048576 == 0)
                printf("nonce=%dM hash=%s\n", genesis.nNonce / 1048576, genesis.GetHash().ToString().c_str());
            genesis.nNonce++;
        }

        printf("hashGenesisBlock=%s\n", genesis.GetHash().ToString().c_str());
        printf("hashMerkleRoot=%s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("genesis.nNonce=%d\n", genesis.nNonce);
#endif

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000007951940bc9c325ac34ef5548d94e450cabcac43f12a3b737a7ccf5480"));
        assert(genesis.hashMerkleRoot == uint256S("0xe8478d9efc59412ca9800e1eb495c324f81a343c1e7c68552cbd39605f9355d1"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        //vSeeds.emplace_back("good_testnet_neonnode.com"); //duplicate this line with more neon nodes

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,142); // neon: addresses begin with 'z'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        //neon: new random keys
        base58Prefixes[EXT_PUBLIC_KEY] = {0x6f, 0x88, 0xd1, 0x6e};
        base58Prefixes[EXT_SECRET_KEY] = {0x6f, 0x12, 0xdc, 0xcc};

        // human readable prefix to bench32 address
        bech32_hrp = "tneon";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {     0, uint256S("0x00000007951940bc9c325ac34ef5548d94e450cabcac43f12a3b737a7ccf5480")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 000000003f7d1f4b55d529fdcf3983c3c8a969ab0900dee41bed0bdaf1c3a9d7 (height 0)
            1620866102, // * UNIX timestamp of last known number of transactions
            0,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.001       // * estimated number of transactions per second after that timestamp
                        // 868816/(1588604853-1346029522) = 0.003581633
        };
    }
};

/**
 * Regression test
 */

class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.powLimit =            uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 28;
        consensus.bnInitialHashTarget = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 29;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60; // two weeks
        consensus.nStakeTargetSpacing = 10 * 60; // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;

        consensus.nStakeMinAge = 60 * 60 * 24; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        //neon: new random pch
        pchMessageStart[0] = 0x9d;
        pchMessageStart[1] = 0xa0;
        pchMessageStart[2] = 0x8b;
        pchMessageStart[3] = 0xff;

        nDefaultPort = 19442;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1620794568, 1620794168, 345397033u, 0x1d0fffff, 1, 0);

#ifdef GENESIS
        arith_uint256 target;
        target.SetCompact(genesis.nBits);
        while (UintToArith256(genesis.GetHash()) > target) {
            if (genesis.nNonce % 1048576 == 0)
                printf("nonce=%dM hash=%s\n", genesis.nNonce / 1048576, genesis.GetHash().ToString().c_str());
            genesis.nNonce++;
        }

        printf("hashGenesisBlock=%s\n", genesis.GetHash().ToString().c_str());
        printf("hashMerkleRoot=%s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("genesis.nNonce=%d\n", genesis.nNonce);
#endif

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000028fe310b417f07b7086b16fcee100c150afc5f0542bad1e6d04866349"));
        assert(genesis.hashMerkleRoot == uint256S("0x607a14d98119861430ae4c95425cc9c9497ee9305d570f84736d8d5441ec228d"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        m_is_test_chain = true;
        m_is_mockable_chain = true;
        checkpointData = {
            {
                {0, uint256S("0x000000028fe310b417f07b7086b16fcee100c150afc5f0542bad1e6d04866349")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,142);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        //neon: new random keys
        base58Prefixes[EXT_PUBLIC_KEY] = {0x20, 0x46, 0x4f, 0x7f};
        base58Prefixes[EXT_SECRET_KEY] = {0x20, 0xf1, 0xec, 0xdf};

        bech32_hrp = "rneon";
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        //fMineBlocksOnDemand = true;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }
 }

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
