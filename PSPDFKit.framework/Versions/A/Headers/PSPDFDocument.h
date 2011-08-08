//
//  PSPDFDocument.h
//  PSPDFKit
//
//  Created by Peter Steinberger on 7/20/11.
//  Copyright 2011 Peter Steinberger. All rights reserved.
//

#import "PSPDFCache.h"

@class PSPDFDocumentSearcher, PSPDFOutlineParser, PSPDFPageInfo, PSPDFAnnotationParser;

/// Represents a single, logical, pdf document. (one or many pdf files)
/// Can be overriden to support custom collections.
@interface PSPDFDocument : NSObject {
    NSURL *basePath_;
    NSArray *files_;
    NSString *uid_;
    NSString *title_;
    
    BOOL aspectRatioEqual_;
    BOOL searchEnabled_;
    BOOL outlineEnabled_;
    BOOL annotationsEnabled_;
    BOOL isDestroyed_;
    BOOL displayed_;
    NSUInteger pageCount_;
    NSMutableDictionary *pageInfoCache_;
    NSMutableDictionary *pageCountCache_;
    
    PSPDFDocumentSearcher *documentSearcher_;
    PSPDFOutlineParser *outlineParser_;
    PSPDFAnnotationParser *annotationParser_;
}

/// initialize empty PSPDFDocument 
+ (PSPDFDocument *)PDFDocument;

/// initalize PSPDFDocument with distinct path and an array of files
+ (PSPDFDocument *)PDFDocumentWithBaseUrl:(NSURL *)baseUrl files:(NSArray *)files;

/// initializes PSPDFDocument with a single file
+ (PSPDFDocument *)PDFDocumentWithUrl:(NSURL *)url;

- (id)init;
- (id)initWithUrl:(NSURL *)url;
- (id)initWithBaseUrl:(NSURL *)basePath files:(NSArray *)files;

/// returns path for a single page (in case pages are split up). Page starts at 0.
- (NSURL *)pathForPage:(NSUInteger)page;

/// return pdf page count
- (NSUInteger)pageCount;

/// return pdf page number. this may be different if a collection of pdfs is used a one big document. Page starts at 0.
- (NSUInteger)pageNumberForPage:(NSUInteger)page;

/// cached rotation and aspect ratio data for specific page. Page starts at 0.
- (PSPDFPageInfo *)pageInfoForPage:(NSUInteger)page;

/// aspect ratio is automatically cached and analyzed per page. Page starts at 0.
/// maybe needs a pdf lock if not already cached.
- (CGRect)rectBoxForPage:(NSUInteger)page;

/// rotation for specified page. cached. Page starts at 0.
- (int)rotationForPage:(NSUInteger)page;

/// if you change internal properties (like file count), cache needs to be cleared
- (void)clearCache;

/// creats internal cache for faster display. override to provide custom caching. called in a thread.
- (void)fillCache;

/// return plain thumbnail path, if thumbnail already exists. override if you pre-provide thumbnails. Returns nil on default.
- (NSURL *)thumbnailPathForPage:(NSUInteger)page;

/// return true if you want drawOverlayRect to be called.
- (BOOL)shouldDrawOverlayRectForSize:(PSPDFSize)size;

/// can be overridden to draw an overlay on the pdf - will be called from drawing threads!
- (void)drawOverlayRect:(CGRect)rect inContext:(CGContextRef)context forPage:(NSUInteger)page zoomScale:(CGFloat)zoomScale size:(PSPDFSize)size;

/// defaults to nil. can be overridden to provide custom text.
/// if this returns nil for a site, we'll try to extract text ourselves
- (NSString *)pageContentForPage:(NSUInteger)page;

/// document title as shown in the controller
@property(nonatomic, copy) NSString *title;

/// For caching, provide a unique uid here
@property(nonatomic, copy) NSString *uid;

/// common base path for pdf files
@property(nonatomic, retain) NSURL *basePath;

/// array of pdf files
@property(nonatomic, copy) NSArray *files;

/// if aspect ratio is equal on all pages, you can enable this for even better performance. Defaults to NO.
@property(nonatomic, assign, getter=isAspectRatioEqual, readonly) BOOL aspectRatioEqual;

/// text extraction is not possible for all pdfs. disable search if not working. Defaults to YES.
@property(nonatomic, assign, getter=isSearchEnabled) BOOL searchEnabled;

/// outline/table of contents extraction. disable if no outline available. Defaults to YES.
@property(nonatomic, assign, getter=isOutlineEnabled) BOOL outlineEnabled;

/// annotation link extraction. Defaults to YES.
@property(nonatomic, assign, getter=isAnnotationsEnabled) BOOL annotationsEnabled;

/// returns if document is currently displayed by a PSPDFViewController. Don't set this yourself. Optimizes caching.
@property(nonatomic, assign, getter=isDisplayed) BOOL displayed;

/// Text extraction class for current document. Readonly.
@property(nonatomic, retain, readonly) PSPDFDocumentSearcher *documentSearcher;

/// Outline extraction class for current document. Readonly.
@property(nonatomic, retain, readonly) PSPDFOutlineParser *outlineParser;

/// Link annotation parser class for current document. Readonly.
@property(nonatomic, retain, readonly) PSPDFAnnotationParser *annotationParser;

@end
