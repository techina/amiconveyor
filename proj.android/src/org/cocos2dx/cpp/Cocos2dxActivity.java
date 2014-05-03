package org.cocos2dx.cpp;

import java.io.File;
import java.util.Hashtable;

import net.uracon.gsweeper.BuildConfig;
import jp.co.septeni.original.leadblow.FacebookActivity;
import net.uracon.gsweeper.R;
import jp.co.septeni.original.leadblow.billing.IabHelper;
import jp.co.septeni.original.leadblow.billing.IabHelper.OnIabPurchaseFinishedListener;
import jp.co.septeni.original.leadblow.billing.Purchase;
import jp.co.septeni.original.leadblow.billing.IabHelper.OnIabSetupFinishedListener;
import jp.co.septeni.original.leadblow.billing.IabResult;
import jp.co.septeni.original.leadblow.gameservice.GameHelper;
import jp.co.septeni.original.leadblow.gameservice.GameHelper.GameHelperListener;
import android.app.NativeActivity;
import android.content.ComponentName;
import android.content.ContentValues;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.view.Gravity;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.facebook.LoggingBehavior;
import com.facebook.Session;
import com.facebook.Session.NewPermissionsRequest;
import com.facebook.Session.StatusCallback;
import com.facebook.SessionState;
import com.facebook.Settings;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.games.Games;

import com.flurry.android.FlurryAgent;

public class Cocos2dxActivity extends NativeActivity{

	private static Cocos2dxActivity that = null;
	private static final String PERMISSION = "publish_actions";
	private IabHelper iabHelper;
	private GameHelper gameHelper;
	public static int IAB_REQUEST_CODE = 10001;
	public static int LEADERBOARD_REQUEST_CODE = 10002;
	private Handler handler;
  private WindowManager wm;
  private AdView adView;
  private boolean showBoardOnSignIn = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		that = this;
		handler = new Handler();

		Settings.addLoggingBehavior(LoggingBehavior.INCLUDE_ACCESS_TOKENS);
        Session session = Session.getActiveSession();
        if (session == null) {
            if (savedInstanceState != null) {
                session = Session.restoreSession(this, null, null, savedInstanceState);
            }
            if (session == null) {
                session = new Session(this);
            }
            Session.setActiveSession(session);
        }

        String base64PublicKey = getString(R.string.iab_key);
        iabHelper = new IabHelper(this, base64PublicKey);
		iabHelper.enableDebugLogging(BuildConfig.DEBUG);
        iabHelper.startSetup(new OnIabSetupFinishedListener() {
			@Override
			public void onIabSetupFinished(IabResult result) {
				if (result.isFailure()) {
					// fail
				}
			}
		});

        gameHelper =  new GameHelper(this, GameHelper.CLIENT_GAMES);
        gameHelper.enableDebugLog(BuildConfig.DEBUG);
        gameHelper.setup(new GameHelperListener() {
            @Override
            public void onSignInSucceeded() {
                if (showBoardOnSignIn) {
                    showLeaderBoard();
                }
                showBoardOnSignIn = false;
                int highScore = getSharedPreferences("Cocos2dxPrefsFile", 0).getInt("highScore", -1);
                if (highScore > 0) {
                    Games.Leaderboards.submitScore(gameHelper.getApiClient(), "CgkI7LrTpPUJEAIQAQ", highScore);
                }
            }
            @Override
            public void onSignInFailed() {
                showBoardOnSignIn = false;
            }
        });

        // WindowManagerを取得する
        wm = (WindowManager) getSystemService(WINDOW_SERVICE);

        FlurryAgent.setLogEnabled(BuildConfig.DEBUG);

		//For supports translucency
		
		//1.change "attribs" in cocos\2d\platform\android\nativeactivity.cpp
		/*const EGLint attribs[] = {
	            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  
	            //EGL_BLUE_SIZE, 5,   -->delete 
	            //EGL_GREEN_SIZE, 6,  -->delete 
	            //EGL_RED_SIZE, 5,    -->delete 
	            EGL_BUFFER_SIZE, 32,  //-->new field
	            EGL_DEPTH_SIZE, 16,
	            EGL_STENCIL_SIZE, 8,
	            EGL_NONE
	    };*/
		
		//2.Set the format of window
		// getWindow().setFormat(PixelFormat.TRANSLUCENT);
		
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (iabHelper.handleActivityResult(requestCode, resultCode, data)) {
			return;
		}
		super.onActivityResult(requestCode, resultCode, data);
		Session.getActiveSession().onActivityResult(this, requestCode,
				resultCode, data);
    showAdView();
    gameHelper.onActivityResult(requestCode, resultCode, data);
	}

  @Override
  protected void onStart() {
      super.onStart();
      FlurryAgent.onStartSession(this, "XK5G85PB9JXCX5XV2TYM");
      gameHelper.onStart(this);
  }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        Session session = Session.getActiveSession();
        Session.saveSession(session, outState);
    }

    @Override
    public void onResume() {
        super.onResume();
        showAdView();
        if (adView != null) {
            adView.resume();
        }
    }

    @Override
    public void onPause() {
        if (adView != null) {
            adView.pause();
        }
        super.onPause();
    }

    @Override
    public void onStop() {
        super.onStop();
        hideAdView();
        FlurryAgent.onEndSession(this);
        gameHelper.onStop();
    }

    @Override
    public void onDestroy() {
        // Destroy the AdView.
        if (adView != null) {
            wm.removeView(adView);
            adView.destroy();
        }
        super.onDestroy();
    }

    private void showAdView() {
        if (adView != null) {
            return;
        }
        adView = new AdView(this);
        adView.setAdSize(AdSize.BANNER);
        adView.setAdUnitId("ca-app-pub-9353254478629065/4014378631");
        handler.post(new Runnable() {
            public void run() {
                /*
                int wc = FrameLayout.LayoutParams.WRAP_CONTENT;
                FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(wc, wc);
                params.gravity = (Gravity.BOTTOM | Gravity.CENTER);
                addContentView(adView, params);
                */
                WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams();
                layoutParams.gravity = Gravity.BOTTOM | Gravity.CENTER;
                layoutParams.x       = 0;
                layoutParams.y       = 0;
                layoutParams.width   = WindowManager.LayoutParams.WRAP_CONTENT;
                layoutParams.height  = WindowManager.LayoutParams.WRAP_CONTENT;
                layoutParams.type    = WindowManager.LayoutParams.TYPE_TOAST;  // ゲーム画面より前面に表示
                layoutParams.flags   = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;  // 自分以外のところがタッチされたとき、背後のビューにタッチイベントを渡す。

                // WindowManagerにAdMobのビューを追加する。
                wm.addView(adView, layoutParams);
                AdRequest adRequest = null;
                if (BuildConfig.DEBUG) {
                    adRequest = new AdRequest.Builder()
            .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
            //.addTestDevice("INSERT_YOUR_HASHED_DEVICE_ID_HERE")
            .build();
                } else {
                    adRequest = new AdRequest.Builder().build();
                }
                adView.loadAd(adRequest);
            }
        });
    }

    private void hideAdView() {
        handler.post(new Runnable() {
            public void run() {
                if (null != adView) {
                    if (null != wm) {
                        wm.removeView(adView);
                    }
                    adView.destroy();
                    adView = null;
                }
            }
        });
    }

	public static void tweet(String message, String filePath) {
		File file = new File(filePath);
		file.setReadable(true, false);
		// if needed
		// that.registerMedia(file);
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("image/jpeg");
		intent.putExtra(Intent.EXTRA_TEXT, message);
		intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(file));
		String packageName = "com.twitter.android";
		PackageManager pm = that.getPackageManager();
		for (ResolveInfo info :pm.queryIntentActivities(intent, 0)) {
			if (info.activityInfo.applicationInfo.packageName.equals(packageName)) {
				intent.setComponent(new ComponentName(packageName, info.activityInfo.name));
				that.startActivity(intent);
				return;
			}
		}
		that.startActivity(Intent.createChooser(intent, "共有"));
	}
	
	private void registerMedia(File file) {
		ContentValues values = new ContentValues();
		values.put(Images.Media.MIME_TYPE, "image/jpeg");
		values.put("_data", file.getAbsolutePath());
		getContentResolver().insert(
				MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
	}
	
	public static void facebook(String message, String filePath) {
		that.tryPost(filePath, message);
	}

	private void tryPost(String filePath, String message) {
		Session session = Session.getActiveSession();
		if (session.isOpened()) {
			if (session.getPermissions().contains(PERMISSION)) {
				showPostActivity(message, filePath);
			} else {
				requestPermission(session, filePath, message);
			}
		} else {
			login(session, filePath, message);
		}
	}

	private void showPostActivity(String message, String filePath) {
		Intent intent = new Intent(that, FacebookActivity.class);
		intent.putExtra("message", message);
		intent.putExtra("filePath", filePath);
		that.startActivity(intent);
	}

	private void requestPermission(Session session, final String filePath,
			final String message) {
		session.requestNewPublishPermissions(new NewPermissionsRequest(this,
				PERMISSION).setCallback(new StatusCallback() {
			@Override
			public void call(Session session, SessionState state,
					Exception exception) {
				if (state == SessionState.OPENED_TOKEN_UPDATED) {
					tryPost(filePath, message);
				}
			}
		}));
	}

	private void login(Session session, final String filePath, final String message) {
		StatusCallback callback = new StatusCallback() {
			@Override
			public void call(Session session, SessionState state, Exception exception) {
				if (state == SessionState.OPENED) {
					tryPost(filePath, message);
				}
			}
		};
        if (!session.isClosed()) {
            session.openForRead(new Session.OpenRequest(this).setCallback(callback));
        } else {
            Session.openActiveSession(this, true, callback);
        }
    }

	public static String getCachePath() {
		return that.getCacheDir().getAbsolutePath();
	}

	public static void openReviewPage() {
		Intent intent = new Intent(Intent.ACTION_VIEW,
				Uri.parse("https://play.google.com/store/apps/details?id="
						+ that.getPackageName()));
		that.startActivity(intent);
	}

	public static void purchase(String productID) {
		that.launchPurchaseFlow(productID);
	}

	public static native void onPurchase(String result);

	private void launchPurchaseFlow(String sku) {
		iabHelper.launchPurchaseFlow(this, sku, IAB_REQUEST_CODE, new OnIabPurchaseFinishedListener() {
			@Override
			public void onIabPurchaseFinished(IabResult result, Purchase info) {
				if (result.isSuccess()) {
					iabHelper.consumeAsync(info, null);
				}
				onPurchase(result.getMessage());
			}
		});
	}

	public static void showGamePlatform() {
		that.showGamePlatform_();
	}

	private void showGamePlatform_() {
      hideAdView();
		handler.post(new Runnable() {
			@Override
			public void run() {
				if (gameHelper.isSignedIn()) {
					showLeaderBoard();
				} else {
            showBoardOnSignIn = true;
					gameHelper.beginUserInitiatedSignIn();
				}
			}
		});
	}

  private void showLeaderBoard() {
      Intent intent = Games.Leaderboards.getLeaderboardIntent(gameHelper.getApiClient(), "CgkI7LrTpPUJEAIQAQ");
      startActivityForResult(intent, LEADERBOARD_REQUEST_CODE);
  }

  public static void reportScore(int score) {
      if (that.gameHelper.isSignedIn()) {
          Games.Leaderboards.submitScore(that.gameHelper.getApiClient(), "CgkI7LrTpPUJEAIQAQ", score);
      }
  }

  public static void logEvent(String eventId, Hashtable<String, String> paramMap) {
      logEvent_(eventId, paramMap, true, false);
  }

  public static void startEvent(String eventId, Hashtable<String, String> paramMap) {
      logEvent_(eventId, paramMap, true, true);
  }

  public static void endEvent(String eventId, Hashtable<String, String> paramMap) {
      logEvent_(eventId, paramMap, false, true);
  }

  private static void logEvent_(String eventId, Hashtable<String, String> paramMap, final boolean start, final boolean timed) {
      final String curId = eventId;
      final Hashtable<String, String> curParam = paramMap;
      that.handler.post(new Runnable() {
          @Override
          public void run() {
              boolean noParam = curParam == null || curParam.isEmpty();
              if (start) {
                  if (timed) {
                      if (noParam) {
                          FlurryAgent.logEvent(curId, true);
                      } else {
                          FlurryAgent.logEvent(curId, curParam, true);
                      }
                  } else {
                      if (noParam) {
                          FlurryAgent.logEvent(curId);
                      } else {
                          FlurryAgent.logEvent(curId, curParam);
                      }
                  }
              } else {
                  if (noParam) {
                      FlurryAgent.endTimedEvent(curId);
                  } else {
                      FlurryAgent.endTimedEvent(curId, curParam);
                  }
              }
          }
      });
  }

}
