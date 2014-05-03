package jp.co.septeni.original.leadblow;

import java.io.File;
import java.io.FileNotFoundException;

import com.facebook.Request;
import com.facebook.Response;
import com.facebook.Session;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.ActionBarActivity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Toast;

import net.uracon.gsweeper.R;

public class FacebookActivity extends ActionBarActivity {

	private static final String FRAGMENT_TAG = "placeholderFragment";
	private String defaultMessage;
	private String filePath;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_facebook);

		if (savedInstanceState == null) {
			getSupportFragmentManager()
					.beginTransaction()
					.add(R.id.container, new PlaceholderFragment(),
							FRAGMENT_TAG).commit();
		}
		getActionBar().setDisplayHomeAsUpEnabled(true);
		getActionBar().setDisplayShowTitleEnabled(false);
		Intent intent = getIntent();
		defaultMessage = intent.getStringExtra("message");
		filePath = intent.getStringExtra("filePath");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.facebook, menu);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		switch (id) {
		case android.R.id.home:
			finish();
			return true;
		case R.id.action_post:
			uploadPhoto();
			finish();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	private void uploadPhoto() {
		PlaceholderFragment fragment = (PlaceholderFragment) getSupportFragmentManager()
				.findFragmentByTag(FRAGMENT_TAG);
		String message = fragment.editText.getText().toString();
		final FacebookActivity that = this;
		Request.Callback callback = new Request.Callback() {
			@Override
			public void onCompleted(Response response) {
				if (response.getError() != null) {
					Toast.makeText(that, response.getError().getErrorMessage(),
							Toast.LENGTH_SHORT).show();
				} else {
					Toast.makeText(that, "uploaded", Toast.LENGTH_SHORT).show();
				}
			}
		};
		if (fragment.imageGone) {
			if (message.length() > 0) {
				Request.newStatusUpdateRequest(Session.getActiveSession(),
						message, callback).executeAsync();
			}
		} else {
			try {
				Request request = Request.newUploadPhotoRequest(
						Session.getActiveSession(), new File(filePath),
						callback);
				if (message.length() > 0) {
					request.getParameters().putString("message", message);
				}
				request.executeAsync();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		private EditText editText;
		private boolean imageGone = false;

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_facebook,
					container, false);
			final FacebookActivity activity = (FacebookActivity) getActivity();
			editText = (EditText) rootView.findViewById(R.id.editText1);
			editText.setText(activity.defaultMessage);
			editText.setSelection(activity.defaultMessage.length());
			final ImageView iv = (ImageView) rootView
					.findViewById(R.id.imageView1);
			iv.setImageURI(Uri.fromFile(new File(activity.filePath)));
			final ImageButton ib = (ImageButton) rootView
					.findViewById(R.id.imageButton1);
			ib.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					iv.setVisibility(View.GONE);
					ib.setVisibility(View.GONE);
					imageGone = true;
				}
			});
			return rootView;
		}
	}

}
