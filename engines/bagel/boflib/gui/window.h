
/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BAGEL_BOFLIB_GUI_WINDOW_H
#define BAGEL_BOFLIB_GUI_WINDOW_H

#include "common/list.h"
#include "common/events.h"
#include "bagel/boflib/array.h"
#include "bagel/boflib/stdinc.h"
#include "bagel/boflib/error.h"
#include "bagel/boflib/object.h"
#include "bagel/boflib/point.h"
#include "bagel/boflib/rect.h"
#include "bagel/boflib/size.h"
#include "bagel/boflib/timer.h"
#include "bagel/boflib/llist.h"
#include "bagel/boflib/gfx/bitmap.h"
#include "bagel/boflib/gfx/palette.h"

namespace Bagel {

#define MAX_TITLE 64
#define USE_DEFAULT -1

class CBofBitmap;
class CBofTimerPacket;

class CBofWindow : public CLList, public CBofObject, public CBofError {
private:
	bool _visible = true;
	bool _enabled = true;
	Common::List<WindowTimer> _timers;
	uint32 _lastLButtonTime = 0, _lastRButtonTime = 0;

	/**
	 * Handles translating from a ScummVM event structure to
	 * a code used by the game engine
	 */
	uint32 TranslateKey(const Common::Event &event) const;

protected:
	/**
	 * Checks window timers for expiry
	 */
	void CheckTimers();

public:
	/**
	 * Default constructor
	 */
	CBofWindow();

	/**
	 * Constructor for CBofWindow
	 * @param pszName   Name of window
	 * @param x         X position
	 * @param y         Y position
	 * @param nWidth    Width of window to create (optional)
	 * @paramnHeight    Height of window to create (optional)
	 * @param pParent   Parent of this window (optional)
	 */
	CBofWindow(const char *pszName, int x = 0, int y = 0, int nWidth = USE_DEFAULT, int nHeight = USE_DEFAULT, CBofWindow *pParent = nullptr);

	/**
	 * Destructor
	 */
	virtual ~CBofWindow();

	static ErrorCode initialize();
	static ErrorCode shutdown();
	static Common::Point getMousePos();

	/**
	 * Creates a window
	 * @param pszName       Name of window
	 * @param x             X position of upper-left corner
	 * @param y             Y position of upper-left corner
	 * @param nWidth        Width of window to create (optional)
	 * @param nHeight       Height of window to create (optional)
	 * @param pParent       Parent of this window (optional)
	 * @param nControlID    User defined ID of this window
	 * @return              Error return code
	 */
	ErrorCode Create(const char *pszName, int x = 0, int y = 0, int nWidth = USE_DEFAULT, int nHeight = USE_DEFAULT, CBofWindow *pParent = nullptr, uint32 nControlID = 0);

	/**
	 * Creates a window
	 * @param pszName       Name of window
	 * @param pRect         Rectangle for window placement
	 * @param pParent       Parent of this window (optional)
	 * @paramnControlID     User defined ID of this window
	 * @return              Error return code
	 */
	ErrorCode Create(const char *pszName, const CBofRect *pRect = nullptr, CBofWindow *pParent = nullptr, uint32 nControlID = 0);

	/**
	 * Destroys the Window attached to this CBofWindow (if any)
	 */
	virtual void Destroy();
	virtual void DestroyWindow() {
		Destroy();
	}

	/**
	 * Shows current window (if hidden)
	 */
	void Show();

	void Select();

	/**
	 * Hides current window (if shown)
	 */
	void Hide();

	/**
	 * Centers current window in parent window or in screen
	 */
	void Center();

	/**
	 * Moves current window to specified location in parent
	 * @param x         New upper left corner X position
	 * @param y         New upper left corner Y position
	 * @param bRepaint  true if should update the window
	 */
	void Move(const int x, const int y, bool bRepaint = false);

	/**
	 * Resizes current window to specified area
	 * @param pRect     New area for window
	 * @parambRepaint   Optional repaint after resize
	 */
	void ReSize(CBofRect *pRect, bool bRepaint = false);

	void Close() {
		OnClose();
	}

	/**
	 * Posts a message
	 * @param nMessage      Message to post
	 * @param lParam1       User info
	 * @param lParam2       More user info
	 */
	void PostMessage(uint32 nMessage, uint32 lParam1, uint32 lParam2);

	/**
	 * Posts a user defined message
	 */
	void PostUserMessage(uint32 nMessage, uint32 lExtraInfo);

	/**
	 * Sets a timer which calls specified callback (or OnTimer)
	 * @param nID           ID of timer to set
	 * @param nInterval     Number of milliseconds till event
	 * @param pCallBack     Function to call when time is up
	 */
	void SetTimer(uint32 nID, uint32 nInterval, BofCallback pCallBack = nullptr);

	/**
	 * Stops specified timer
	 * @param nTimerID       ID of timer to stop
	 */
	void KillTimer(uint32 nTimerID);

	/**
	 * Stops all timers associated with current window
	 */
	void KillMyTimers();

	/**
	 * Determines if specified window is a child to current window
	 * @param pWin      Window to check
	 * @return          true if pWnd is a child of current window, false if not
	 */
	bool IsChildOf(CBofWindow *pWin);

	/**
	 * Determines if specified window is a parent to current window
	 * @param pWin      Window to check
	 * @return          true if pWnd is a parent of current window, false if not
	 */
	bool IsParentOf(CBofWindow *pWin);

	/**
	 * Returns the parent window element, if any
	*/
	CBofWindow *GetParent() const {
		return _parent;
	}
	CBofWindow *GetAnscestor();

	/**
	 * Causes all parent windows to have valid paint regions
	 * @param pRect         Area to validate
	 */
	void ValidateAnscestors(CBofRect *pRect = nullptr);

	static CBofWindow *GetActiveWindow() {
		return m_pActiveWindow;
	}

	void SetActive() {
		m_pActiveWindow = this;
	}

	static CBofWindow *GetWindowList() {
		return m_pWindowList;
	}

	CBofRect GetWindowRect() const {
		return m_cWindowRect;
	}
	CBofRect GetClientRect();

	CBofRect GetRect() const {
		return m_cRect;
	}

	int Width() const {
		return m_cRect.Width();
	}
	int Height() const {
		return m_cRect.Height();
	}

	void ScreenToClient(CBofPoint *pPoint);
	void ClientToScreen(CBofPoint *pPoint);

	/**
	 * Selects and Realizes specified palette into current DC
	 * @param pPal      Palette to select
	 */
	void SelectPalette(CBofPalette *pPal);

	/**
	 * Associates a new background bitmap to this window
	 * @param pBitmap       New background bitmap
	 * @param bRefresh      true if should repaint now
	 * @return              Error return code
	 */
	ErrorCode SetBackdrop(CBofBitmap *pBitmap, bool bRefresh = false);

	/**
	 * Associates a new background bitmap to this window
	 * @param pszBmpFile    new background bitmap from file
	 * @param bRefresh      true if should repaint now
	 * @return              Error return code
	 */
	ErrorCode SetBackdrop(const char *pszBmpFile, bool bRefresh = false);

	void ClearBackdrop() {
		m_pBackdrop = nullptr;
	}

	CBofBitmap *GetBackdrop() {
		return m_pBackdrop;
	}

	bool HasBackdrop() {
		return m_pBackdrop != nullptr;
	}

	/**
	 * Deletes the background bitmap associated with this window
	 */
	void KillBackdrop();

	/**
	 * Updates the specified section of the background bitmap
	 * @param pRect     Area of bitmap to update on screen
	 * @return          Error return code
	 */
	ErrorCode PaintBackdrop(CBofRect *pRect = nullptr, int nTransparentColor = -1);

	void SetControlID(uint32 nID) {
		m_nID = nID;
	}
	uint32 GetControlID() {
		return m_nID;
	}

	void SetBkColor(RGBCOLOR cColor) {
		m_cBkColor = cColor;
	}
	RGBCOLOR GetBkColor() {
		return m_cBkColor;
	}

	void SetFgColor(RGBCOLOR cColor) {
		m_cFgColor = cColor;
	}
	RGBCOLOR GetFgColor() {
		return m_cFgColor;
	}

	void SetPrevMouseDown(CBofPoint p) {
		m_cPrevMouseDown = p;
	}
	CBofPoint GetPrevMouseDown() {
		return m_cPrevMouseDown;
	}

	/**
	 * Sets mouse capture for this window
	 */
	void SetCapture();

	/**
	 * Release mouse capture for this window
	 */
	void ReleaseCapture();

	/**
	 * Returns true if the control is capturing mouse events
	 */
	bool HasCapture() const;

	/**
	 * Sets the focus on a control for keyboard input
	 */
	void SetFocus();

	/**
	 * Releases focus from an edit control
	 */
	void ReleaseFocus();

	/**
	 * Returns true if the control has focus
	 */
	bool HasFocus() const;

	void FlushAllMessages();

	/**
	 * Adds specified rectangle to dirty rect list for this window
	 * @param pRect     Rectangle to add to dirty list
	 */
	void ValidateRect(const CBofRect *pRect);

	/**
	 * Removes specified rectangle from dirty rect for this window
	 * @param pRect     Rectangle to remove from dirty list
	 */
	void InvalidateRect(const CBofRect *pRect);

	virtual void OnBofButton(CBofObject *pButton, int nExtraInfo);
	virtual void OnBofScrollBar(CBofObject *pButton, int nNewPos);
	virtual void OnBofListBox(CBofObject *pListBox, int nItemIndex);
	virtual void OnMainLoop();

	virtual void OnSoundNotify(CBofObject *pObject, uint32 lParam2);
	virtual void OnMovieNotify(uint32 lParam1, uint32 lParam2);

	virtual void OnMCINotify(uint32 lParam1, uint32 lParam2);

	virtual void OnTimer(uint32 nTimerId);

	/**
	 * Handles a pending ScummVM event
	 * @param event		Event to process
	 */
	virtual void handleEvent(const Common::Event &event);

	Graphics::ManagedSurface *getSurface();

	bool IsCreated() const {
		return _surface != nullptr;
	}

	virtual void Enable() {
		_enabled = true;
		UpdateWindow();
	}
	virtual void Disable() {
		_enabled = false;
		UpdateWindow();
	}
	bool IsVisible() const {
		return _visible;
	}
	bool IsEnabled() const {
		return _enabled;
	}

	void UpdateWindow();

	void setParent(CBofWindow *parent);

	/**
	 * Handle all pending ScummVM events
	 */
	void handleEvents();

	virtual void OnKeyHit(uint32 lKey, uint32 lRepCount);
	void FillWindow(byte iColor);
	void FillRect(CBofRect *pRect, byte iColor);

protected:
	CBofWindow *_parent = nullptr;	// Pointer to parent window
	Array<CBofWindow *> _children;	// Child element pointers

	virtual void OnMouseMove(uint32 nFlags, CBofPoint *pPoint, void * = nullptr);
	virtual void OnLButtonDown(uint32 nFlags, CBofPoint *pPoint, void * = nullptr);
	virtual void OnLButtonUp(uint32 nFlags, CBofPoint *pPoint, void * = nullptr);
	virtual void OnLButtonDblClk(uint32 nFlags, CBofPoint *pPoint);

	virtual void OnRButtonDown(uint32 nFlags, CBofPoint *pPoint);
	virtual void OnRButtonUp(uint32 nFlags, CBofPoint *pPoint);
	virtual void OnRButtonDblClk(uint32 nFlags, CBofPoint *pPoint);

	virtual void OnReSize(CBofSize *pSize);
	virtual void OnPaint(CBofRect *pRect);
	virtual void OnClose();

	virtual void OnUserMessage(uint32 nMessage, uint32 lParam);

	virtual void OnActivate();
	virtual void OnDeActivate();

	// Window Data
	char m_szTitle[MAX_TITLE] = { 0 };	// Title of window
	CBofRect m_cWindowRect;				// Screen based area of this window
	CBofRect m_cRect;					// Window-based area of this window
	CBofBitmap *m_pBackdrop = nullptr;  // Backdrop bitmap
	uint32 m_nID = 0;						// ID of this window

	RGBCOLOR m_cBkColor = RGB(255, 255, 255);
	RGBCOLOR m_cFgColor = RGB(0, 0, 0);

	bool m_bCaptured = false;
	Graphics::ManagedSurface *_surface = nullptr;

	static CBofWindow *m_pWindowList;
	static CBofWindow *m_pActiveWindow;
	static CBofTimerPacket *m_pTimerList;
	CBofPoint m_cPrevMouseDown;
	static int _mouseX;
	static int _mouseY;
};

class CBofMessage : public CBofObject {
public:
	CBofWindow *m_pWindow; // destination window for message
	uint32 m_nMessage;      // message to send (usually WM_USER)
	uint32 m_lParam1;       // user defined info
	uint32 m_lParam2;       // more user defined info
};

class CBofTimerPacket : public CBofObject, public CLList {
public:
	CBofWindow *m_pOwnerWindow;
	BofCallback m_pCallBack;
	uint32 m_nID;
	uint32 m_nInterval;
};

extern CBofWindow *g_hackWindow;

} // namespace Bagel

#endif
