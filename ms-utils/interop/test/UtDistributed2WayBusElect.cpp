/**
@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

//#include <iostream>
#include "UtDistributed2WayBusElect.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtDistributed2WayBusElect::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtDistributed2WayBusElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDistributed2WayBusElect::UtDistributed2WayBusElect()
    :
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtDistributed2WayBusElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDistributed2WayBusElect::~UtDistributed2WayBusElect()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::setUp()
{
    /// - Default construct the nominal test article.
    tArticle = new FriendlyDistributed2WayBusElect;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of the Electrical Distributed 2-Way Bus notification message.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testNotificationConstruction()
{
    UT_RESULT_FIRST;

    /// @test Default construction.
    Distributed2WayBusNotification defaultNotif;
    CPPUNIT_ASSERT(Distributed2WayBusNotification::NONE == defaultNotif.mLevel);
    CPPUNIT_ASSERT(""                                        == defaultNotif.mMessage);

    /// @test Nominal construction.
    Distributed2WayBusNotification nominalNotif(Distributed2WayBusNotification::ERR, "Error!");
    CPPUNIT_ASSERT(Distributed2WayBusNotification::ERR == nominalNotif.mLevel);
    CPPUNIT_ASSERT("Error!"                                 == nominalNotif.mMessage);

    /// @test Copy construction.
    Distributed2WayBusNotification copyNotif(nominalNotif);
    CPPUNIT_ASSERT(Distributed2WayBusNotification::ERR == copyNotif.mLevel);
    CPPUNIT_ASSERT("Error!"                                 == copyNotif.mMessage);

    /// @test Assignment operator.
    defaultNotif = copyNotif;
    CPPUNIT_ASSERT(Distributed2WayBusNotification::ERR == defaultNotif.mLevel);
    CPPUNIT_ASSERT("Error!"                                 == defaultNotif.mMessage);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the Distributed2WayBusElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(0                                  == tArticle->mInData.mFrameCount);
    CPPUNIT_ASSERT(0                                  == tArticle->mInData.mFrameLoopback);
    CPPUNIT_ASSERT(false                              == tArticle->mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0F                               == tArticle->mInData.mDemandPower);
    CPPUNIT_ASSERT(0.0F                               == tArticle->mInData.mSupplyVoltage);
    CPPUNIT_ASSERT(0                                  == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(0                                  == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false                              == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0F                               == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0.0F                               == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(false                              == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(Distributed2WayBusElect::NONE == tArticle->mForcedRole);
    CPPUNIT_ASSERT(0                                  == tArticle->mSupplyDatas.size());
    CPPUNIT_ASSERT(0                                  == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(0                                  == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0                                  == tArticle->mNotifications.size());

    /// @test    New/delete for code coverage.
    Distributed2WayBusElect* testArticle = new Distributed2WayBusElect();
    delete testArticle;

    /// @test    Interface data functions.
    Distributed2WayBusElectInterfaceData* ifData1 = new Distributed2WayBusElectInterfaceData();
    Distributed2WayBusElectInterfaceData* ifData2 = new Distributed2WayBusElectInterfaceData();
    ifData1->mDemandMode    = true;
    ifData1->mDemandPower   = 1.0;
    ifData1->mFrameCount    = 42;
    ifData1->mFrameLoopback = 12;
    ifData1->mSupplyVoltage = 120.0;
    *ifData2 = *ifData1;
    CPPUNIT_ASSERT(ifData2->mDemandMode    == ifData1->mDemandMode);
    CPPUNIT_ASSERT(ifData2->mDemandPower   == ifData1->mDemandPower);
    CPPUNIT_ASSERT(ifData2->mFrameCount    == ifData1->mFrameCount);
    CPPUNIT_ASSERT(ifData2->mFrameLoopback == ifData1->mFrameLoopback);
    CPPUNIT_ASSERT(ifData2->mSupplyVoltage == ifData1->mSupplyVoltage);
    CPPUNIT_ASSERT(true                    == ifData2->hasValidData());
    delete ifData2;
    delete ifData1;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface initialization without errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testNominalInitialization()
{
    UT_RESULT;

    /// @test    Adding some voltage supply interfaces.
    Distributed2WayBusElectSupplyData* supply1 = tArticle->createSupplyData();
    Distributed2WayBusElectSupplyData* supply2 = tArticle->createSupplyData();
    Distributed2WayBusElectSupplyData* supply3 = tArticle->createSupplyData();

    CPPUNIT_ASSERT(3       == tArticle->mSupplyDatas.size());
    CPPUNIT_ASSERT(supply1 == tArticle->mSupplyDatas.at(0));
    CPPUNIT_ASSERT(supply2 == tArticle->mSupplyDatas.at(1));
    CPPUNIT_ASSERT(supply3 == tArticle->mSupplyDatas.at(2));

    /// @test    Initialize function as primary side.
    const float voltage = 120.0F;
    tArticle->initialize(true, voltage);

    CPPUNIT_ASSERT(true    == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(true    == tArticle->mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0F    == tArticle->mInData.mDemandPower);
    CPPUNIT_ASSERT(voltage == tArticle->mInData.mSupplyVoltage);
    CPPUNIT_ASSERT(false   == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0F    == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(voltage == tArticle->mOutData.mSupplyVoltage);

    /// @test    Initialize function as secondary side.
    FriendlyDistributed2WayBusElect article2;
    article2.initialize(false, voltage);

    CPPUNIT_ASSERT(false   == article2.mIsPairMaster);
    CPPUNIT_ASSERT(false   == article2.mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0F    == article2.mInData.mDemandPower);
    CPPUNIT_ASSERT(voltage == article2.mInData.mSupplyVoltage);
    CPPUNIT_ASSERT(true    == article2.mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0F    == article2.mOutData.mDemandPower);
    CPPUNIT_ASSERT(voltage == article2.mOutData.mSupplyVoltage);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface nominal initialization with
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testInitializationErrors()
{
    UT_RESULT;

    /// - currently nothing to test

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface updateFrameCounts method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testUpdateFrameCounts()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(true, 120.0F);

    /// @test    updateFrameCounts method.
    tArticle->mOutData.mFrameCount   = 43;
    tArticle->mInData.mFrameCount    = 44;
    tArticle->mInData.mFrameLoopback = 42;
    tArticle->mFramesSinceFlip       = 99;
    tArticle->updateFrameCounts();
    CPPUNIT_ASSERT(44  == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(100 == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(2   == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(44  == tArticle->mOutData.mFrameLoopback);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    Distributed2WayBusElectSupplyData* supply1 = tArticle->createSupplyData();
    Distributed2WayBusElectSupplyData* supply2 = tArticle->createSupplyData();
    tArticle->initialize(true, 120.0F);
    tArticle->mOutData.mFrameCount = 43;

    Distributed2WayBusNotification notif;
    Distributed2WayBusNotification::NotificationLevel NONE =
            Distributed2WayBusNotification::NONE;
    Distributed2WayBusNotification::NotificationLevel INFO =
            Distributed2WayBusNotification::INFO;

    /// @test    Remain in Supply mode.
    supply1->mAvailable              = true;
    supply1->mMaximumVoltage         = 120.0F;
    supply2->mAvailable              = true;
    supply2->mMaximumVoltage         = 105.0F;
    tArticle->mInData.mDemandMode    = true;
    tArticle->mInData.mDemandPower   = 100.0F;
    tArticle->mInData.mSupplyVoltage = 110.0F;
    tArticle->mInData.mFrameCount    = 44;
    tArticle->mInData.mFrameLoopback = 42;
    tArticle->updateFrameCounts();
    tArticle->update(119.0F, 1.0F);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(false  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(119.0F == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(0.0F   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(NONE   == notif.mLevel);
    CPPUNIT_ASSERT(""     == notif.mMessage);

    /// @test    Switch to Demand mode.
    supply1->mAvailable              = false;
    tArticle->mInData.mFrameCount    = 45;
    tArticle->mInData.mFrameLoopback = 43;
    tArticle->updateFrameCounts();
    tArticle->update(104.0, 1.0);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(true   == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(105.0F == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(1.0F   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0      == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(INFO   == notif.mLevel);
    CPPUNIT_ASSERT(0      == notif.mMessage.rfind("flipping to Demand role", 0));

    /// @test    Remain in Demand mode, even though our local supply has returned, because not
    ///          enough frames have passed since our flip to Demand.
    supply1->mAvailable              = true;
    tArticle->mInData.mFrameCount    = 46;
    tArticle->mInData.mFrameLoopback = 44;
    tArticle->updateFrameCounts();
    tArticle->update(119.0F, 1.0F);
    tArticle->popNotification(notif);

    tArticle->mInData.mFrameCount    = 47;
    tArticle->mInData.mFrameLoopback = 45;
    tArticle->updateFrameCounts();
    tArticle->update(119.0F, 1.0F);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(true   == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(120.0F == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(1.0F   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(2      == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(NONE   == notif.mLevel);
    CPPUNIT_ASSERT(""     == notif.mMessage);

    /// @test    Switch to Supply mode.
    tArticle->mInData.mFrameCount    = 48;
    tArticle->mInData.mFrameLoopback = 46;
    tArticle->updateFrameCounts();
    tArticle->update(119.0F, 1.0F);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(false  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(119.0F == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(0.0F   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0      == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(INFO   == notif.mLevel);
    CPPUNIT_ASSERT(0      == notif.mMessage.rfind("flipping to Supply role", 0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface update method with forced roles.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testUpdateForcedRole()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(true, 120.0F);
    tArticle->mOutData.mFrameCount = 43;

    /// @test    Remains in Supply when forced.
    tArticle->mInData.mDemandMode    = true;
    tArticle->mInData.mDemandPower   = 100.0F;
    tArticle->mInData.mSupplyVoltage = 110.0F;
    tArticle->mInData.mFrameCount    = 44;
    tArticle->mInData.mFrameLoopback = 42;
    tArticle->updateFrameCounts();
    tArticle->forceSupplyRole();
    tArticle->update(2.0F, 1.0F);

    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(2.0F  == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(0.0F  == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0     == tArticle->mNotifications.size());

    /// @test    Force to Demand role.
    tArticle->forceDemandRole();
    tArticle->update(2.0F, 1.0F);

    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0F  == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(1.0F  == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0     == tArticle->mNotifications.size());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Distributed 2-Way Bus Interface getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusElect::testAccessors()
{
    UT_RESULT;

    /// @test    resetForceRole()
    tArticle->mForcedRole = Distributed2WayBusElect::SUPPLY;
    tArticle->resetForceRole();
    CPPUNIT_ASSERT(Distributed2WayBusElect::NONE == tArticle->mForcedRole);

    /// @test    isInDemandRole()
    tArticle->mOutData.mDemandMode = true;
    CPPUNIT_ASSERT(true  == tArticle->isInDemandRole());
    tArticle->mOutData.mDemandMode = false;
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());

    /// @test    getRemoteLoad()
    tArticle->mInData.mDemandPower = 10.0F;
    tArticle->mOutData.mDemandMode = true;
    CPPUNIT_ASSERT(0.0F  == tArticle->getRemoteLoad());
    tArticle->mOutData.mDemandMode = false;
    CPPUNIT_ASSERT(10.0F == tArticle->getRemoteLoad());

    /// @test    getRemoteSupply()
    tArticle->mInData.mSupplyVoltage = 100.0F;
    CPPUNIT_ASSERT(100.0F == tArticle->getRemoteSupply());

    UT_PASS_FINAL;
}
