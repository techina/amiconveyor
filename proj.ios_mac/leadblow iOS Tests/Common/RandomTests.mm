//
//  RandomTests.m
//  leadblow
//
//  Created by n_totani on 2014/03/12.
//
//

#import <XCTest/XCTest.h>
#import "RandomMock.h"

@interface RandomTests : XCTestCase

@end

@implementation RandomTests

- (void)setUp
{
    [super setUp];
    // Put setup code here; it will be run once, before the first test case.
}

- (void)tearDown
{
    // Put teardown code here; it will be run once, after the last test case.
    [super tearDown];
}

- (void)testConstruct
{
    RandomMock* rnd = new RandomMock({1, 2, 3});
    XCTAssertEqual(1, rnd->next());
    XCTAssertEqual(2, rnd->next());
    XCTAssertEqual(3, rnd->next());
    XCTAssertEqual(1, rnd->next());
}

- (void)testConstructDefault
{
    RandomMock* rnd = new RandomMock();
    XCTAssertEqual(0, rnd->next());
    XCTAssertEqual(0, rnd->next());
    XCTAssertEqual(0, rnd->next());
    XCTAssertEqual(0, rnd->next());
}

@end
