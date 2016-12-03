#include "BaseScene.h"
#include "HomeScene.h"

Scene* BaseScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	BaseScene *layer = BaseScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

#ifdef SDKBOX_ENABLED
class IAdmobListener : public sdkbox::AdMobListener {
public:
	virtual void adViewDidReceiveAd(const std::string &name) {
		CCLog("bambi admob adViewDidReceiveAd %s: ", name.c_str());
		//sdkbox::PluginAdMob::show(name);
	}
	virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {
		CCLog("bambi admob adViewDidFailToReceiveAdWithError %s: ", msg.c_str());
	}
	virtual void adViewWillPresentScreen(const std::string &name) {
		CCLog("bambi admob adViewWillPresentScreen %s: ", name.c_str());
	}
	virtual void adViewDidDismissScreen(const std::string &name) {
		CCLog("bambi admob adViewDidDismissScreen %s: ", name.c_str());
	}
	virtual void adViewWillDismissScreen(const std::string &name) {
		sdkbox::PluginAdMob::cache(name);
		CCLog("bambi admob adViewWillDismissScreen %s: ", name.c_str());
	}
	virtual void adViewWillLeaveApplication(const std::string &name) {
		CCLog("bambi admob adViewWillLeaveApplication %s: ", name.c_str());
	}
};
class IChartboostListener : public sdkbox::ChartboostListener {
public:
	virtual void onChartboostCached(const std::string& name)
	{
		HomeScene::getInstance()->isChartboostAdsAvailable = true;
		CCLog("bambi chartboost onChartboostCached %s: ", name.c_str());
	}
	virtual bool onChartboostShouldDisplay(const std::string& name)
	{
		CCLog("bambi chartboost onChartboostShouldDisplay %s: ", name.c_str());
	}
	virtual void onChartboostDisplay(const std::string& name)
	{
		CCLog("bambi chartboost onChartboostDisplay %s: ", name.c_str());
	}
	virtual void onChartboostDismiss(const std::string& name)
	{
		CCLog("bambi chartboost onChartboostDismiss %s: ", name.c_str());
	}
	virtual void onChartboostClose(const std::string& name)
	{
		CCLog("bambi chartboost onChartboostClose %s: ", name.c_str());
	}
	virtual void onChartboostClick(const std::string& name)
	{
		CCLog("bambi chartboost onChartboostClick %s: ", name.c_str());
	}
	virtual void onChartboostReward(const std::string& name, int reward)
	{
		CCLog("bambi chartboost onChartboostReward %s: ", name.c_str());

		HomeScene::getInstance()->isChartboostAdsAvailable = false;
		//TODO return to the listener
		HomeScene::getInstance()->onVideoAdsPlayed();
	}
	virtual void onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e)
	{
		CCLog("bambi chartboost onChartboostFailedToLoad %s: ", name.c_str());
		auto func = CallFunc::create([=]() {
					sdkbox::PluginChartboost::show(kChartboostRewardedAds);
				});
		HomeScene::getInstance()->runAction(Sequence::create(DelayTime::create(1), func, nullptr));
	}
	virtual void onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e)
	{
		CCLog("bambi chartboost onChartboostFailToRecordClick %s: ", name.c_str());
	}
	virtual void onChartboostConfirmation()
	{
		CCLog("bambi chartboost onChartboostConfirmation");
	}
	virtual void onChartboostCompleteStore()
	{
		CCLog("bambi chartboost onChartboostCompleteStore");
	}
};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
class IVungleListener : public sdkbox::VungleListener, public Ref
{
public:
	virtual void onVungleCacheAvailable()
	{
		HomeScene::getInstance()->isVungleAdsAvailable = true;
		CCLog("bambi vungle onVungleCacheAvailable");
	}
	virtual void onVungleStarted()
	{
		CCLog("bambi vungle onVungleStarted");
	}
	virtual void onVungleFinished()
	{
		CCLog("bambi vungle onVungleFinished");
	}
	virtual void onVungleAdViewed(bool isComplete)
	{
		CCLog("bambi vungle onVungleAdViewed - isComplete: %s",isComplete?"true":"false");
	}
	virtual void onVungleAdReward(const std::string& name) {
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
					HomeScene::getInstance()->isVungleAdsAvailable = false;
					HomeScene::getInstance()->onVideoAdsPlayed();
					CCLog("bambi vungle onVungleAdReward - name: %s",name.c_str());
				});
	}
};
#endif
#endif
void BaseScene::showFullscreenAds() {
	CCLog("bambi showFullscreenAds");
//#ifdef SDKBOX_ENABLED
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	int random = CppUtils::randomBetween(1,3);
//	if(random == 1)
//	{
//		sdkbox::PluginChartboost::show(kChartboostInstitialAds);
//	} else if(random == 2) {
//		sdkbox::PluginAdMob::show(kAdmobInstitialAds);
//	} else {
//		sdkbox::PluginVungle::show(kVungleInstitialAds);
//	}
//#else
//	int random = CppUtils::randomBetween(1,2);
//	if(random == 1)
//	{
//		sdkbox::PluginChartboost::show(kChartboostInstitialAds);
//	} else {
//		sdkbox::PluginAdMob::show(kAdmobInstitialAds);
//	}
//#endif
//#endif
}

//bool BaseScene::isRewardedAdsAvailable() {
//	bool result = false;
//#ifdef SDKBOX_ENABLED
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	result = isChartboostAdsAvailable || isVungleAdsAvailable;
//	if(isChartboostAdsAvailable)
//	{
//		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> chartboost ads is available");
//	} else
//	{
//		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> chartboost ads is not available");
//	}
//
//	if(isVungleAdsAvailable)
//	{
//		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> vungle ads is available");
//	} else
//	{
//		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> vungle ads is not available");
//	}
//#else
//	result = sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds);
//#endif
//#endif
//	return result;
//}

bool BaseScene::isRewardedAdsAvailable() {
	bool result = false;
#ifdef SDKBOX_ENABLED
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	result = sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds) || sdkbox::PluginVungle::isCacheAvailable();
	if(!sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds))
	{
		sdkbox::PluginChartboost::cache(kChartboostRewardedAds);
	}

	if(sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds))
	{
		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> chartboost ads is available");
	} else
	{
		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> chartboost ads is not available");
	}

	if(sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds))
	{
		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> chartboost ads is available");
	} else
	{
		CCLog("bambi HomeScene -> isRewardedAdsAvailable -> chartboost ads is not available");
	}
#else
	result = sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds);
	if(!result)
	{
		sdkbox::PluginChartboost::cache(kChartboostRewardedAds);
	}
#endif
#endif
	return result;
}

bool isShowingAds = false;
void BaseScene::showRewardedAds() {
	if (isShowingAds) {
		return;
	}
	isShowingAds = true;
	auto func = CallFunc::create([=]() {
		isShowingAds = false;
	});
	this->runAction(Sequence::create(DelayTime::create(1), func, nullptr));

	CCLog("bambi showRewardedAds");
#ifdef SDKBOX_ENABLED
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(CppUtils::randomBetween(1,2) == 1) {
		if(sdkbox::PluginChartboost::isAvailable(kChartboostRewardedAds))
		{
			sdkbox::PluginChartboost::show(kChartboostRewardedAds);
		} else
		{
			sdkbox::PluginVungle::show(kVungleRewardedAds);
		}
	} else {
		if(sdkbox::PluginVungle::isCacheAvailable())
		{
			sdkbox::PluginVungle::show(kVungleRewardedAds);
		} else
		{
			sdkbox::PluginChartboost::show(kChartboostRewardedAds);
		}
	}
#else
	sdkbox::PluginChartboost::show(kChartboostRewardedAds);
#endif

#endif
}
// on "init" you need to initialize your instance
bool BaseScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	isChartboostAdsAvailable = false;
	isVungleAdsAvailable = false;
	isSound = UserDefault::getInstance()->getBoolForKey(SOUND, true);
	UserDefault::getInstance()->setBoolForKey(SOUND, isSound);
	isMusic = UserDefault::getInstance()->getBoolForKey(MUSIC, true);
	UserDefault::getInstance()->setBoolForKey(MUSIC, isMusic);
	isAlert = UserDefault::getInstance()->getBoolForKey(ALERT, true);
	UserDefault::getInstance()->setBoolForKey(ALERT, isAlert);

	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();
#ifdef SDKBOX_ENABLED
	sdkbox::PluginAdMob::setListener(new IAdmobListener());
	sdkbox::PluginChartboost::setListener(new IChartboostListener());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginVungle::setListener(new IVungleListener());
	sdkbox::PluginVungle::setDebug(true);
#endif
#endif
	return true;
}

