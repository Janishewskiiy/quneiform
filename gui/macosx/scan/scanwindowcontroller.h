/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

typedef void (*scannedCallback)(const char *);

@interface ScanWindowController : NSWindowController<NSWindowDelegate, IKScannerDeviceViewDelegate> {
    IBOutlet IKScannerDeviceView * devView;
    IBOutlet IKDeviceBrowserView * devBrowse;
    scannedCallback callback_;
}

- (id)initWithCallback:(scannedCallback) callback;
- (void)showDialog;
// NSWindowDelegate
- (void)windowWillClose:(NSNotification *)notification;
// IKScannerDeviceViewDelegate
- (void)scannerDeviceView:(IKScannerDeviceView *)scannerDeviceView didScanToURL:(NSURL *)url fileData:(NSData *)data error:(NSError *)error;

@end