package uk.co.realh.hgame.android;

import android.view.KeyEvent;

import uk.co.realh.hgame.DInput;

/**
 * Created by tony on 18/03/15.
 * A DInput which uses the keyboard.
 */
public class AndroidKbdDInput implements DInput, KeyEvent.Callback
{
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent keyEvent)
    {
        int mask = getMaskForKeyCode(keyCode);
        if (mask != 0)
        {
            keyMask |= mask;
        }
        return mask != 0;
    }

    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent keyEvent)
    {
        return false;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent keyEvent)
    {
        int mask = getMaskForKeyCode(keyCode);
        if (mask != 0)
        {
            keyMask &= ~mask;
        }
        return mask != 0;
    }

    @Override
    public boolean onKeyMultiple(int keyCode, int count, KeyEvent keyEvent)
    {
        return false;
    }

    @Override
    public int pollDInput()
    {
        int mask = 0;
        if ((keyMask & KEYS_LEFT_MASK) != 0)
        {
            mask |= LEFT;
        }
        if ((keyMask & KEYS_UP_MASK) != 0)
        {
            mask |= UP;
        }
        if ((keyMask & KEYS_RIGHT_MASK) != 0)
        {
            mask |= RIGHT;
        }
        if ((keyMask & KEYS_DOWN_MASK) != 0)
        {
            mask |= DOWN;
        }
        return mask;
    }

    private int getMaskForKeyCode(int keyCode)
    {
        switch (keyCode)
        {
            case KeyEvent.KEYCODE_DPAD_LEFT:
                return KEY_LEFT_ARROW_MASK;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                return KEY_RIGHT_ARROW_MASK;
            case KeyEvent.KEYCODE_DPAD_UP:
                return KEY_UP_ARROW_MASK;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                return KEY_DOWN_ARROW_MASK;
            case KeyEvent.KEYCODE_Z:
                return KEY_Z_MASK;
            case KeyEvent.KEYCODE_L:
                return KEY_L_MASK;
            case KeyEvent.KEYCODE_X:
                return KEY_X_MASK;
            case KeyEvent.KEYCODE_COMMA:
                return KEY_COMMA_MASK;
            case KeyEvent.KEYCODE_A:
                return KEY_A_MASK;
            case KeyEvent.KEYCODE_W:
                return KEY_W_MASK;
            case KeyEvent.KEYCODE_D:
                return KEY_D_MASK;
            case KeyEvent.KEYCODE_S:
                return KEY_S_MASK;
            default:
                return 0;
        }
    }

    private int keyMask;

    static final int KEY_LEFT_ARROW_MASK = 1;
    static final int KEY_UP_ARROW_MASK = 2;
    static final int KEY_RIGHT_ARROW_MASK = 4;
    static final int KEY_DOWN_ARROW_MASK = 8;
    static final int KEY_Z_MASK = 16;
    static final int KEY_L_MASK = 32;
    static final int KEY_COMMA_MASK = 64;
    static final int KEY_X_MASK = 128;
    static final int KEY_A_MASK = 256;
    static final int KEY_W_MASK = 512;
    static final int KEY_D_MASK = 1024;
    static final int KEY_S_MASK = 2048;

    static final int KEYS_LEFT_MASK = KEY_LEFT_ARROW_MASK | KEY_Z_MASK | KEY_A_MASK;
    static final int KEYS_UP_MASK = KEY_UP_ARROW_MASK | KEY_L_MASK | KEY_W_MASK;
    static final int KEYS_RIGHT_MASK = KEY_RIGHT_ARROW_MASK | KEY_X_MASK | KEY_D_MASK;
    static final int KEYS_DOWN_MASK = KEY_DOWN_ARROW_MASK | KEY_COMMA_MASK | KEY_S_MASK;
}
