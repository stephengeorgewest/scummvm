package org.scummvm.scummvm;

import android.os.Handler;
import android.os.Message;
import android.content.Context;
//import android.util.Log;
import android.util.Log;
import android.view.KeyEvent;
import android.view.KeyCharacterMap;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewConfiguration;
import android.view.GestureDetector;
import android.view.InputDevice;
//import android.view.inputmethod.InputMethodManager;

import androidx.annotation.NonNull;

import java.lang.ref.WeakReference;

public class ScummVMEventsBase implements
		android.view.View.OnKeyListener,
		android.view.View.OnTouchListener,
		android.view.GestureDetector.OnGestureListener,
		android.view.GestureDetector.OnDoubleTapListener {

	public static final int JE_SYS_KEY = 0;
	public static final int JE_KEY = 1;
	public static final int JE_DPAD = 2;
	public static final int JE_DOWN = 3;
	public static final int JE_SCROLL = 4;
	public static final int JE_TAP = 5;
	public static final int JE_DOUBLE_TAP = 6;
	public static final int JE_MULTI = 7;
	public static final int JE_BALL = 8;
	public static final int JE_LMB_DOWN = 9;
	public static final int JE_LMB_UP = 10;
	public static final int JE_RMB_DOWN = 11;
	public static final int JE_RMB_UP = 12;
	public static final int JE_MOUSE_MOVE = 13;
	public static final int JE_GAMEPAD = 14;
	public static final int JE_JOYSTICK = 15;
	public static final int JE_MMB_DOWN = 16;
	public static final int JE_MMB_UP = 17;
	public static final int JE_BMB_DOWN = 18;
	public static final int JE_BMB_UP = 19;
	public static final int JE_FMB_DOWN = 20;
	public static final int JE_FMB_UP = 21;
	public static final int JE_QUIT = 0x1000;

	final protected Context _context;
	final protected ScummVM _scummvm;
	final protected GestureDetector _gd;
	final protected int _longPress;
	final protected MouseHelper _mouseHelper;

	// Custom handler code (to avoid mem leaks, see warning "This Handler Class Should Be Static Or Leaks Might Occur”) based on:
	// https://stackoverflow.com/a/27826094
	public static class ScummVMEventHandler extends Handler {

		private final WeakReference<ScummVMEventsBase> mListenerReference;

		public ScummVMEventHandler(ScummVMEventsBase listener) {
			mListenerReference = new WeakReference<>(listener);
		}

		@Override
		public synchronized void handleMessage(@NonNull Message msg) {
			ScummVMEventsBase listener = mListenerReference.get();
			if(listener != null) {
				listener.handleEVHMessage(msg);
			}
		}

		public void clear() {
			this.removeCallbacksAndMessages(null);
		}
	}

	final private ScummVMEventHandler _skeyHandler = new ScummVMEventHandler(this);

//	/**
//	 * An example getter to provide it to some external class
//	 * or just use 'new MyHandler(this)' if you are using it internally.
//	 * If you only use it internally you might even want it as final member:
//	 * private final MyHandler mHandler = new MyHandler(this);
//	 */
//	public Handler ScummVMEventHandler() {
//		return new ScummVMEventHandler(this);
//	}

	public ScummVMEventsBase(Context context, ScummVM scummvm, MouseHelper mouseHelper) {
		_context = context;
		_scummvm = scummvm;
		_mouseHelper = mouseHelper;

		_gd = new GestureDetector(context, this);
		_gd.setOnDoubleTapListener(this);
		_gd.setIsLongpressEnabled(false);

		_longPress = ViewConfiguration.getLongPressTimeout();

	}

	private void handleEVHMessage(final Message msg) {
		if (msg.what == MSG_SMENU_LONG_PRESS) {
			// this toggles the android keyboard (see showVirtualKeyboard() in ScummVMActivity.java)
			// when menu key is long-pressed
//			InputMethodManager imm = (InputMethodManager)
//				_context.getSystemService(Context.INPUT_METHOD_SERVICE);
//
//			if (imm != null)
//				imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
			((ScummVMActivity) _context).toggleScreenKeyboard();
		} else if (msg.what == MSG_SBACK_LONG_PRESS) {
			_scummvm.pushEvent(JE_SYS_KEY, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_MENU, 0, 0, 0, 0);
			_scummvm.pushEvent(JE_SYS_KEY, KeyEvent.ACTION_UP, KeyEvent.KEYCODE_MENU, 0, 0, 0, 0);
		}
	}

	public void clearEventHandler() {
		_skeyHandler.clear();
	}

	final public void sendQuitEvent() {
		_scummvm.pushEvent(JE_QUIT, 0, 0, 0, 0, 0, 0);
	}

	public boolean onTrackballEvent(MotionEvent e) {
		_scummvm.pushEvent(JE_BALL, e.getAction(),
							(int)(e.getX() * e.getXPrecision() * 100),
							(int)(e.getY() * e.getYPrecision() * 100),
							0, 0, 0);
		return true;
	}

	public boolean onGenericMotionEvent(MotionEvent e) {
		return false;
	}

	final static int MSG_SMENU_LONG_PRESS = 1;
	final static int MSG_SBACK_LONG_PRESS = 2;

	// OnKeyListener
	@Override
	final public boolean onKey(View v, int keyCode, KeyEvent e) {
		//Log.d(ScummVM.LOG_TAG, "SCUMMV-EVENTS-BASE - onKEY");
		final int action = e.getAction();

		if (e.getUnicodeChar() == (int)EditableAccommodatingLatinIMETypeNullIssues.ONE_UNPROCESSED_CHARACTER.charAt(0)) {
			//We are ignoring this character, and we want everyone else to ignore it, too, so
			// we return true indicating that we have handled it (by ignoring it).
			return true;
		}

		if (keyCode == 238) {
			// this (undocumented) event is sent when ACTION_HOVER_ENTER or ACTION_HOVER_EXIT occurs
			return false;
		}

		if (keyCode == KeyEvent.KEYCODE_BACK) {
			if (_mouseHelper != null) {
				if (MouseHelper.isMouse(e)) {
					// right mouse button was just clicked which sends an extra back button press (which should be ignored)
					return true;
				}
			}

			if (ScummVMActivity.keyboardWithoutTextInputShown ) {
				if (action == KeyEvent.ACTION_DOWN) {
					return true;
				} else if (action == KeyEvent.ACTION_UP) {
					// Hide keyboard (the argument here (0) does not matter)
					((ScummVMActivity) _context).showScreenKeyboardWithoutTextInputField(0);
					return true;
				}
			}
		}

		if (e.isSystem()) {
			// no repeats for system keys
			if (e.getRepeatCount() > 0)
				return false;

			// Have to reimplement hold-down-menu-brings-up-softkeybd
			// ourselves, since we are otherwise hijacking the menu key :(
			// See com.android.internal.policy.impl.PhoneWindow.onKeyDownPanel()
			// for the usual Android implementation of this feature.
			//
			// We adopt a similar behavior for the Back system button, as well.
			if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK) {
				//
				// Upon pressing the system menu or system back key:
				// (The example below assumes that system Back key was pressed)
				// 1. keyHandler.hasMessages(MSG_SBACK_LONG_PRESS) = false, and thus: fired = true
				// 2. Action will be KeyEvent.ACTION_DOWN, so a delayed message "MSG_SBACK_LONG_PRESS" will be sent to keyHandler after _longPress time
				//    The "MSG_SBACK_LONG_PRESS" will be handled (and removed) in the keyHandler.
				//    For the Back button, the keyHandler should forward a ACTION_UP for MENU (the alternate func of Back key!) to native)
				//    But if the code enters this section before the "MSG_SBACK_LONG_PRESS" was handled in keyHandler (probably due to a ACTION_UP)
				//        then fired = false and the message is removed from keyHandler, meaning we should treat the button press as a SHORT key press
				final int typeOfLongPressMessage;
				if (keyCode == KeyEvent.KEYCODE_MENU) {
					typeOfLongPressMessage = MSG_SMENU_LONG_PRESS;
				} else { // back button
					typeOfLongPressMessage = MSG_SBACK_LONG_PRESS;
				}

				final boolean fired = !_skeyHandler.hasMessages(typeOfLongPressMessage);

				_skeyHandler.removeMessages(typeOfLongPressMessage);

				if (action == KeyEvent.ACTION_DOWN) {
					_skeyHandler.sendMessageDelayed(_skeyHandler.obtainMessage(typeOfLongPressMessage), _longPress);
					return true;
				} else if (action != KeyEvent.ACTION_UP) {
					return true;
				}

				if (fired) {
					return true;
				}

				// It's still necessary to send a key down event to the backend.
				_scummvm.pushEvent(JE_SYS_KEY, KeyEvent.ACTION_DOWN, keyCode,
							e.getUnicodeChar() & KeyCharacterMap.COMBINING_ACCENT_MASK,
							e.getMetaState(), e.getRepeatCount(),
							(int)(e.getEventTime() - e.getDownTime()));
			}
		}

		// sequence of characters
		if (action == KeyEvent.ACTION_MULTIPLE &&
				keyCode == KeyEvent.KEYCODE_UNKNOWN) {
			final KeyCharacterMap m = KeyCharacterMap.load(e.getDeviceId());
			final KeyEvent[] es = m.getEvents(e.getCharacters().toCharArray());

			if (es == null)
				return true;

			for (KeyEvent s : es) {
				_scummvm.pushEvent(JE_KEY, s.getAction(), s.getKeyCode(),
					s.getUnicodeChar() & KeyCharacterMap.COMBINING_ACCENT_MASK,
					s.getMetaState(), s.getRepeatCount(), 0);
			}

			return true;
		}

		int type;
		switch (keyCode) {
		case KeyEvent.KEYCODE_VOLUME_DOWN:
		case KeyEvent.KEYCODE_VOLUME_UP:
			// We ignore these so that they can be handled by Android.
			return false;
		case KeyEvent.KEYCODE_DPAD_UP:
		case KeyEvent.KEYCODE_DPAD_DOWN:
		case KeyEvent.KEYCODE_DPAD_LEFT:
		case KeyEvent.KEYCODE_DPAD_RIGHT:
		case KeyEvent.KEYCODE_DPAD_CENTER:
			if (e.getSource() == InputDevice.SOURCE_DPAD) {
				type = JE_DPAD;
			} else {
				type = JE_KEY;
			}
			break;
		case KeyEvent.KEYCODE_BUTTON_A:
		case KeyEvent.KEYCODE_BUTTON_B:
		case KeyEvent.KEYCODE_BUTTON_C:
		case KeyEvent.KEYCODE_BUTTON_X:
		case KeyEvent.KEYCODE_BUTTON_Y:
		case KeyEvent.KEYCODE_BUTTON_Z:
		case KeyEvent.KEYCODE_BUTTON_L1:
		case KeyEvent.KEYCODE_BUTTON_R1:
		case KeyEvent.KEYCODE_BUTTON_L2:
		case KeyEvent.KEYCODE_BUTTON_R2:
		case KeyEvent.KEYCODE_BUTTON_THUMBL:
		case KeyEvent.KEYCODE_BUTTON_THUMBR:
		case KeyEvent.KEYCODE_BUTTON_START:
		case KeyEvent.KEYCODE_BUTTON_SELECT:
		case KeyEvent.KEYCODE_BUTTON_MODE:
			type = JE_GAMEPAD;
			break;
		default:
			if (e.isSystem()) {
				type = JE_SYS_KEY;
			} else {
				type = JE_KEY;
			}
			break;
		}

		_scummvm.pushEvent(type, action, keyCode,
					e.getUnicodeChar() & KeyCharacterMap.COMBINING_ACCENT_MASK,
					e.getMetaState(), e.getRepeatCount(),
					(int)(e.getEventTime() - e.getDownTime()));

		return true;
	}

	// OnTouchListener
	@Override
	final public boolean onTouch(View v, final MotionEvent event) {
		//Log.d(ScummVM.LOG_TAG, "SCUMMV-EVENTS-BASE - onTOUCH");

		if (ScummVMActivity.keyboardWithoutTextInputShown
			&& ((ScummVMActivity) _context).isScreenKeyboardShown()
			&& ((ScummVMActivity) _context).getScreenKeyboard().getY() <= event.getY() ) {
			event.offsetLocation(-((ScummVMActivity) _context).getScreenKeyboard().getX(), -((ScummVMActivity) _context).getScreenKeyboard().getY());
			// TODO maybe call the onTouchEvent of something else here?
			((ScummVMActivity) _context).getScreenKeyboard().onTouchEvent(event);
			// correct the offset for continuing handling the event
			event.offsetLocation(((ScummVMActivity) _context).getScreenKeyboard().getX(), ((ScummVMActivity) _context).getScreenKeyboard().getY());
		}

		if (_mouseHelper != null) {
			boolean isMouse = MouseHelper.isMouse(event);
			if (isMouse) {
				// mouse button is pressed
				return _mouseHelper.onMouseEvent(event, false);
			}
		}

		final int action = event.getAction();

		// Deal with LINT warning "ScummVMEvents#onTouch should call View#performClick when a click is detected"
		switch (event.getAction()) {
			case MotionEvent.ACTION_UP:
				v.performClick();
				break;
			case MotionEvent.ACTION_DOWN:
				// fall through
			default:
				break;
		}
		// constants from APIv5:
		// (action & ACTION_POINTER_INDEX_MASK) >> ACTION_POINTER_INDEX_SHIFT
		final int pointer = (action & 0xff00) >> 8;

		if (pointer > 0) {
			_scummvm.pushEvent(JE_MULTI, pointer, action & 0xff, // ACTION_MASK
								(int)event.getX(), (int)event.getY(), 0, 0);
			return true;
		}

		return _gd.onTouchEvent(event);
	}

	// OnGestureListener
	@Override
	final public boolean onDown(MotionEvent e) {
		//Log.d(ScummVM.LOG_TAG, "SCUMMV-EVENTS-BASE - onDONW");
		_scummvm.pushEvent(JE_DOWN, (int)e.getX(), (int)e.getY(), 0, 0, 0, 0);
		return true;
	}

	@Override
	final public boolean onFling(MotionEvent e1, MotionEvent e2,
									float velocityX, float velocityY) {
		//Log.d(ScummVM.LOG_TAG, String.format(Locale.ROOT, "onFling: %s -> %s (%.3f %.3f)",
		//										e1.toString(), e2.toString(),
		//										velocityX, velocityY));

		return true;
	}

	@Override
	final public void onLongPress(MotionEvent e) {
		// disabled, interferes with drag&drop
	}

	@Override
	final public boolean onScroll(MotionEvent e1, MotionEvent e2,
									float distanceX, float distanceY) {
		_scummvm.pushEvent(JE_SCROLL, (int)e1.getX(), (int)e1.getY(),
							(int)e2.getX(), (int)e2.getY(), 0, 0);

		return true;
	}

	@Override
	final public void onShowPress(MotionEvent e) {
	}

	@Override
	final public boolean onSingleTapUp(MotionEvent e) {
		_scummvm.pushEvent(JE_TAP, (int)e.getX(), (int)e.getY(),
							(int)(e.getEventTime() - e.getDownTime()), 0, 0, 0);

		return true;
	}

	// OnDoubleTapListener
	@Override
	final public boolean onDoubleTap(MotionEvent e) {
		return true;
	}

	@Override
	final public boolean onDoubleTapEvent(MotionEvent e) {
		_scummvm.pushEvent(JE_DOUBLE_TAP, (int)e.getX(), (int)e.getY(),
							e.getAction(), 0, 0, 0);

		return true;
	}

	@Override
	final public boolean onSingleTapConfirmed(MotionEvent e) {
		return true;
	}
}
