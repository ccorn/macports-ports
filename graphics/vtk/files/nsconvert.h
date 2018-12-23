#ifndef __NSCONVERT_H
#define __NSCONVERT_H
#import <Cocoa/Cocoa.h>

/*
 * Functions _backingScaleFactor(win) and
 * _convert(Point|Rect|Size)(To|From)Backing(obj, rect)
 * used in place of similarly named NS(Screen|Window|View) methods
 * that may or may not yet exist.
 */
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1070

static inline CGFloat
_backingScaleFactor(NSWindow *win) {
    return [win backingScaleFactor];
}

static inline NSPoint
_convertPointToBacking(NSScreen *screen, const NSPoint &point) {
    return [screen convertPointToBacking:point];
}

static inline NSPoint
_convertPointToBacking(NSWindow *win, const NSPoint &point) {
    return [win convertPointToBacking:point];
}

static inline NSPoint
_convertPointToBacking(NSView *view, const NSPoint &point) {
    return [view convertPointToBacking:point];
}

static inline NSPoint
_convertPointFromBacking(NSScreen *screen, const NSPoint &point) {
    return [screen convertPointFromBacking:point];
}

static inline NSPoint
_convertPointFromBacking(NSWindow *win, const NSPoint &point) {
    return [win convertPointFromBacking:point];
}

static inline NSPoint
_convertPointFromBacking(NSView *view, const NSPoint &point) {
    return [view convertPointFromBacking:point];
}

static inline NSRect
_convertRectToBacking(NSScreen *screen, const NSRect &rect) {
    return [screen convertRectToBacking:rect];
}

static inline NSRect
_convertRectToBacking(NSWindow *win, const NSRect &rect) {
    return [win convertRectToBacking:rect];
}

static inline NSRect
_convertRectToBacking(NSView *view, const NSRect &rect) {
    return [view convertRectToBacking:rect];
}

static inline NSRect
_convertRectFromBacking(NSScreen *screen, const NSRect &rect) {
    return [screen convertRectFromBacking:rect];
}

static inline NSRect
_convertRectFromBacking(NSWindow *win, const NSRect &rect) {
    return [win convertRectFromBacking:rect];
}

static inline NSRect
_convertRectFromBacking(NSView *view, const NSRect &rect) {
    return [view convertRectFromBacking:rect];
}

static inline NSSize
_convertSizeToBacking(NSScreen *screen, const NSSize &size) {
    return [screen convertSizeToBacking:size];
}

static inline NSSize
_convertSizeToBacking(NSWindow *win, const NSSize &size) {
    return [win convertSizeToBacking:size];
}

static inline NSSize
_convertSizeToBacking(NSView *view, const NSSize &size) {
    return [view convertSizeToBacking:size];
}

static inline NSSize
_convertSizeFromBacking(NSScreen *screen, const NSSize &size) {
    return [screen convertSizeFromBacking:size];
}

static inline NSSize
_convertSizeFromBacking(NSWindow *win, const NSSize &size) {
    return [win convertSizeFromBacking:size];
}

static inline NSSize
_convertSizeFromBacking(NSView *view, const NSSize &size) {
    return [view convertSizeFromBacking:size];
}

#else /* MAC_OS_X_VERSION_MIN_REQUIRED < 1070 */
/*
 * Instead of converting to backing store pixel coordinates,
 * just convert to screen coordinates, assuming no rotation.
 */
/*
* In 10.5 and 10.6, NSView "Backing" is called "Base", e.g.
https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/
CocoaViewsGuide/WorkingWithAViewHierarchy/WorkingWithAViewHierarchy.html#
//apple_ref/doc/uid/TP40002978-CH4-SW25
 */
#ifndef NSVIEW_CONVERT_SCREEN
#define NSVIEW_CONVERT_SCREEN (MAC_OS_X_VERSION_MIN_REQUIRED < 1050)
#endif

static inline CGFloat
_backingScaleFactor(NSWindow *win) {
    return [win userSpaceScaleFactor];
}

static inline NSPoint
_convertPointToBacking(NSScreen *screen, const NSPoint &point) {
    return point;
}

static inline NSPoint
_convertPointToBacking(NSWindow *win, const NSPoint &point) {
    return [win convertBaseToScreen:point];     // for NSWindow, Base = Window
}

static inline NSPoint
_convertPointToBacking(NSView *view, const NSPoint &point) {
#if NSVIEW_CONVERT_SCREEN
    NSWindow *win = [view window];
    NSPoint winpoint = [view convertPoint:point toView:nil];
    return _convertPointToBacking(win, winpoint);
#else
    return [view convertPointToBase:point];     // for NSView, Base = Backing
#endif
}

static inline NSPoint
_convertPointFromBacking(NSScreen *screen, const NSPoint &point) {
    return point;
}

static inline NSPoint
_convertPointFromBacking(NSWindow *win, const NSPoint &point) {
    return [win convertScreenToBase:point];     // for NSWindow, Base = Window
}

static inline NSPoint
_convertPointFromBacking(NSView *view, const NSPoint &point) {
#if NSVIEW_CONVERT_SCREEN
    NSWindow *win = [view window];
    NSPoint winpoint = _convertPointFromBacking(win, point);
    return [view convertPoint:winpoint fromView:nil];
#else
    return [view convertPointFromBase:point];   // for NSView, Base = Backing
#endif
}

static inline NSRect
_convertRectToBacking(NSScreen *screen, const NSRect &rect) {
    return rect;
}

static inline NSRect
_convertRectToBacking(NSWindow *win, const NSRect &rect) {
    CGFloat xmin = NSMinX(rect);
    CGFloat xmax = NSMaxX(rect);
    CGFloat ymin = NSMinY(rect);
    CGFloat ymax = NSMaxY(rect);
    NSPoint p[4];
    int i = 0;
    p[i++] = NSMakePoint(xmin, ymin);
    p[i++] = NSMakePoint(xmax, ymin);
    p[i++] = NSMakePoint(xmax, ymax);
    p[i] = NSMakePoint(xmin, ymax);
    NSPoint P = _convertPointToBacking(win, p[i]);
    xmin = xmax = P.x;
    ymin = ymax = P.y;
    while (--i >= 0) {
        P = _convertPointToBacking(win, p[i]);
        if (xmin > P.x) xmin = P.x;
        if (xmax < P.x) xmax = P.x;
        if (ymin > P.y) ymin = P.y;
        if (ymax < P.y) ymax = P.y;
    };
    return NSMakeRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

static inline NSRect
_convertRectToBacking(NSView *view, const NSRect &rect) {
#if NSVIEW_CONVERT_SCREEN
    NSWindow *win = [view window];
    NSRect winrect = [view convertRect:rect toView:nil];
    return _convertRectToBacking(win, winrect);
#else
    return [view convertRectToBase:rect];       // for NSView, Base = Backing
#endif
}

static inline NSRect
_convertRectFromBacking(NSScreen *screen, const NSRect &rect) {
    return rect;
}

static inline NSRect
_convertRectFromBacking(NSWindow *win, const NSRect &rect) {
    CGFloat xmin = NSMinX(rect);
    CGFloat xmax = NSMaxX(rect);
    CGFloat ymin = NSMinY(rect);
    CGFloat ymax = NSMaxY(rect);
    NSPoint p[4];
    int i = 0;
    p[i++] = NSMakePoint(xmin, ymin);
    p[i++] = NSMakePoint(xmax, ymin);
    p[i++] = NSMakePoint(xmax, ymax);
    p[i] = NSMakePoint(xmin, ymax);
    NSPoint P = _convertPointFromBacking(win, p[i]);
    xmin = xmax = P.x;
    ymin = ymax = P.y;
    while (--i >= 0) {
        P = _convertPointFromBacking(win, p[i]);
        if (xmin > P.x) xmin = P.x;
        if (xmax < P.x) xmax = P.x;
        if (ymin > P.y) ymin = P.y;
        if (ymax < P.y) ymax = P.y;
    };
    return NSMakeRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

static inline NSRect
_convertRectFromBacking(NSView *view, const NSRect &rect) {
#if NSVIEW_CONVERT_SCREEN
    NSWindow *win = [view window];
    NSRect winrect = _convertRectFromBacking(win, rect);
    return [view convertRect:winrect fromView:nil];
#else
    return [view convertRectFromBase:rect];     // for NSView, Base = Backing
#endif
}

static inline NSSize
_convertSizeToBacking(NSScreen *screen, const NSSize &size) {
    return size;
}

static inline NSSize
_convertSizeToBacking(NSWindow *win, const NSSize &size) {
    NSRect rect = {{}, size};
    return _convertRectToBacking(win, rect).size;
}

static inline NSSize
_convertSizeToBacking(NSView *view, const NSSize &size) {
#if NSVIEW_CONVERT_SCREEN
    NSWindow *win = [view window];
    NSSize winsize = [view convertSize:size toView:nil];
    return _convertSizeToBacking(win, winsize);
#else
    return [view convertSizeToBase:size];       // for NSView, Base = Backing
#endif
}

static inline NSSize
_convertSizeFromBacking(NSScreen *screen, const NSSize &size) {
    return size;
}

static inline NSSize
_convertSizeFromBacking(NSWindow *win, const NSSize &size) {
    NSRect rect = {{}, size};
    return _convertRectFromBacking(win, rect).size;
}

static inline NSSize
_convertSizeFromBacking(NSView *view, const NSSize &size) {
#if NSVIEW_CONVERT_SCREEN
    NSWindow *win = [view window];
    NSSize winsize = _convertSizeFromBacking(win, size);
    return [view convertSize:winsize fromView:nil];
#else
    return [view convertSizeFromBase:size];     // for NSView, Base = Backing
#endif
}
#endif /* MAC_OS_X_VERSION_MIN_REQUIRED < 1070 */
#endif /*__NSCONVERT_H */
