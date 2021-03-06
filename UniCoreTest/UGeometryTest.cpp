#include "stdafx.h"

#include "../UniCore/UGeometry.h"
#include <cmath>

using namespace uni;

//求直线交点可用.
TEST(UGeometryTest,LineLineIntersection_Works)
{
	StraightLine lineA(15,10,54,29);
	StraightLine lineB(41,4,32,32);
	
	auto result = Intersect(lineA,lineB);

	//result(36.0,20.1)
	ASSERT_EQ(1,result.size());
	ASSERT_FLOAT_EQ(35.812351,result[0].x);
	ASSERT_FLOAT_EQ(20.139351,result[0].y);
}

//直线交点.
//lineA = {points=[2]({x=211.08000 y=591.17896 },{x=299.03000 y=642.29706 }) }
//lineB = {points=[2]({x=191.83101 y=629.68298 },{x=300.69000 y=573.91803 }) }
TEST(UGeometryTest,LineLineIntersection_Works2)
{
	StraightLine lineA(211.08000,591.17896,299.03000,642.29706);
	StraightLine lineB(191.83101,629.68298,300.69000,573.91803);

	auto result = Intersect(lineA,lineB);

	//result(36.0,20.1)
	ASSERT_EQ(1,result.size());
// 	ASSERT_FLOAT_EQ(35.812351,result[0].x);
// 	ASSERT_FLOAT_EQ(20.139351,result[0].y);
}

//平行的直线没有交点.
TEST(UGeometryTest,IntersectParallelLine_NoIntersection)
{
	StraightLine lineA(15,10,54,29);
	StraightLine lineB(30,20,69,39);

	auto result = Intersect(lineA,lineB);

	//result(36.0,20.1)
	ASSERT_EQ(0,result.size());
}

static int round(float a)
{
	return (int)(a+0.5);
}

//直线和贝塞尔曲线相交.
TEST(UGeometryTest,LineBezierIntersection_Works)
{
	StraightLine lineA(53,329,401,371);
	CubicBezierLine lineB(168,272,22,501,368,204,352,416);

	auto result = Intersect(lineA,lineB);

	ASSERT_EQ(3,result.size());
	ASSERT_EQ(346,round(result[0].x));
	ASSERT_EQ(364,round(result[0].y));
	ASSERT_EQ(134,round(result[1].x));
	ASSERT_EQ(339,round(result[1].y));
	ASSERT_EQ(217,round(result[2].x));
	ASSERT_EQ(349,round(result[2].y));

	//result t(0.109,0.484,0.896).
}

//直线和贝塞尔曲线相交2.
//straight = {points=[2]({x=407.88898 y=779.05499 },{x=407.88898 y=709.05499 }) }
//bezier = {points=[4]({x=371.71402 y=739.22205 },{x=397.60101 y=746.52502 },{x=429.13000 y=747.18896 },{x=469.28799 y=737.89404 }) }
TEST(UGeometryTest,LineBezierIntersection_Works2)
{
	StraightLine lineA(407.88898,779.05499,407.88898,709.05499);
	CubicBezierLine lineB(371.71402,739.22205,397.60101,746.52502,429.13000,747.18896,469.28799,737.89404);

	auto result = Intersect(lineA,lineB);

	ASSERT_EQ(1,result.size());
}

//直线和贝塞尔曲线相交3.
//straight = {points=[2]({x=413.50000 y=645.75000 },{x=413.50000 y=571.75000 }) }
//bezier = {points=[4]({x=388.25000 y=638.00000 },{x=430.50000 y=625.75000 },{x=433.00000 y=586.50000 },{x=390.00000 y=574.25000 }) }
TEST(UGeometryTest,LineBezierIntersection_Works3)
{
	StraightLine lineA(413.50000,645.75000,413.50000,571.75000);
	CubicBezierLine lineB(388.25000,638.00000,430.50000,625.75000,433.00000,586.50000,390.00000,574.25000);

	auto result = Intersect(lineA,lineB);

	ASSERT_EQ(2,result.size());
}


TEST(UGeometryTest,StraightLineProjection_Works)
{
	StraightLine line(2,2,5,5);
	Point p(0,2);
	Point point = line.getProjectionPoint(p);
	ASSERT_EQ(1,point.x);
	ASSERT_EQ(1,point.y);
}

//投射到水平线段.
TEST(UGeometryTest,StraightLineProjection_Works1)
{
	StraightLine line(2,2,8,2);
	Point p(0,2);
	Point point = line.getNearestPoint(p);
	EXPECT_EQ(2,point.x);
	EXPECT_EQ(2,point.y);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//投射到垂直线段.
TEST(UGeometryTest,StraightLineProjection_Works2)
{
	StraightLine line(2,2,2,-8);
	Point p(0,0);
	Point point = line.getProjectionPoint(p);
	EXPECT_EQ(2,point.x);
	EXPECT_EQ(0,point.y);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

TEST(UGeometryTest,StraightLineNearestPoint_Works)
{
	StraightLine line(2,2,5,5);
	Point p(0,2);
	Point point = line.getNearestPoint(p);
	ASSERT_EQ(2,point.x);
	ASSERT_EQ(2,point.y);
}

TEST(UGeometryTest,StraightLineProjection_Works3)
{
	StraightLine line(71.68,738.22,141.05,738.55);
	Point p(92.46,739.40);
	Point point = line.getProjectionPoint(p);
	ASSERT_GT(point.x,71.68);
	ASSERT_LT(point.x,141.05);
}

TEST(UGeometryTest,StraightLineProjection_Works4)
{
	StraightLine line(-4.78,-2.10,-7.34,1.68);
	Point p(-5.98,-2.43);
	Point point = line.getProjectionPoint(p);
	ASSERT_NEAR(-5.00,point.x,0.01);
	ASSERT_NEAR(-1.77,point.y,0.01);
}

TEST(UGeometryTest,CubicBezierLine_split_Works)
{
	CubicBezierLine line(120,160,35,200,220,260,220,40);
	auto result = line.split(0.1);
	EXPECT_EQ(2,result.size());
	EXPECT_NEAR(0.0f,result[0].originBeginT,0.001);
	EXPECT_NEAR(0.1f,result[0].originEndT,0.001);
	EXPECT_NEAR(0.1f,result[1].originBeginT,0.001);
	EXPECT_NEAR(1.0f,result[1].originEndT,0.001);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//直线可以写成Ax + By + C = 0.
//(y1 – y2)x + (x2 – x1)y + (x1y2 – x2y1) = 0
//A = y1 - y2; B = x2 - x1; C = x1y2 – x2y1;
TEST(UGeometryTest,StraightLine_GetABC_Works)
{
	StraightLine l(2,1,5,7);
	EXPECT_NEAR(-6,l.a(),0.001);
	EXPECT_NEAR(3,l.b(),0.001);
	EXPECT_NEAR(9,l.c(),0.001);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//贝塞尔曲线相交.
TEST(UGeometryTest,Bezier_Intersection_Works)
{
	CubicBezierLine a(10,100,90,30,40,140,220,240);
	CubicBezierLine b(5,150,180,20,80,280,210,190);
	std::vector<Point> result = IntersectBezierAndBezierLine(a,b);
	EXPECT_EQ(3,result.size());
	float x1 = a.getX(0.895);
	float y1 = a.getY(0.895);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//lineA = {points=[4]({x=29.869799 y=749.17999 },{x=82.259300 y=769.42804 },{x=126.58900 y=732.07898 },{x=96.247299 y=685.11597 }) }
//lineB = {points=[4]({x=51.442501 y=774.73901 },{x=46.796101 y=729.59601 },{x=67.041199 y=693.41504 },{x=136.40599 y=706.02899 }) }//贝塞尔曲线相交.
TEST(UGeometryTest,Bezier_Intersection_Works1)
{
	CubicBezierLine a(29.869799,749.17999,82.259300,769.42804,126.58900,732.07898,96.247299,685.11597);
	CubicBezierLine b(51.442501,774.73901,46.796101,729.59601,67.041199,693.41504,136.40599,706.02899);
	std::vector<Point> result = IntersectBezierAndBezierLine(a,b);
	EXPECT_EQ(2,result.size());
	float x1 = a.getX(0.895);
	float y1 = a.getY(0.895);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//tightBoundingRect可用.
TEST(UGeometryTest,tightBoundingBox_Works)
{
	CubicBezierLine a(10,100,90,30,40,140,220,240);
	XRect rect = a.tightBoundingBox();

	EXPECT_GT(rect.points[0].x,0);
	EXPECT_GT(rect.points[0].y,0);
	EXPECT_GT(rect.points[1].x,0);
	EXPECT_GT(rect.points[1].y,0);
	EXPECT_GT(rect.points[2].x,0);
	EXPECT_GT(rect.points[2].y,0);
	EXPECT_GT(rect.points[3].x,0);
	EXPECT_GT(rect.points[3].y,0);

	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//直线的Bounding Box.
TEST(UGeometryTest,StraightLine_BoundingBox_Ok)
{
	StraightLine line(2,-2,-5,0);
	EXPECT_EQ(-5,line.boundingBox().l);
	EXPECT_EQ(-2,line.boundingBox().t);
	EXPECT_EQ(2,line.boundingBox().r);
	EXPECT_EQ(0,line.boundingBox().b);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}



//贝塞尔曲线相交,性能测试.
TEST(UGeometryPerfTest,Bezier_Intersection_Perf)
{
	CubicBezierLine a(10,100,90,30,40,140,220,240);
	CubicBezierLine b(5,150,180,20,80,280,210,190);
	std::vector<Point> result = IntersectBezierAndBezierLine(a,b);
 	for(int i = 0; i < 100; i++)
 	{
 		result = IntersectBezierAndBezierLine(a,b);
 	}
	EXPECT_EQ(3,result.size());
	float x1 = a.getX(0.895);
	float y1 = a.getY(0.895);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//直线和直线相交,性能测试.
TEST(UGeometryPerfTest,LineLineIntersection_Perf)
{
	StraightLine lineA(15,10,54,29);
	StraightLine lineB(41,4,32,32);

	auto result = Intersect(lineA,lineB);
// 	for(int i = 0; i < 6000000; i++)
// 	{
// 		result = Intersect(lineA,lineB);
// 		if(result.size() >= 5900000)
// 		{
// 			break;
// 		}
// 	}

	//result(36.0,20.1)
// 	ASSERT_EQ(1,result.size());
// 	ASSERT_FLOAT_EQ(35.812351,result[0].x);
// 	ASSERT_FLOAT_EQ(20.139351,result[0].y);
}

//分割贝塞尔曲线.
TEST(UGeometryTest,Bezier_split_test1)
{
	CubicBezierLine cubic(442.81189,599.61261,533.04486,576.13934,
		623.64673,636.77960,636.44458,729.12103);
	std::vector<CubicBezierLine> subCurve = cubic.split(1.0359697e-007,0.99999994);
	ASSERT_EQ(3,subCurve.size());
}

//分割贝塞尔曲线,贝塞尔曲线的四个点都为0.0.
TEST(UGeometryTest,Bezier_split_test2)
{
	CubicBezierLine cubic(0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
	std::vector<CubicBezierLine> subCurve = cubic.split(0.5);
	EXPECT_EQ(2,subCurve.size());
	EXPECT_EQ(0.0,subCurve[0].originBeginT);
	EXPECT_EQ(0.5,subCurve[0].originEndT);
	EXPECT_EQ(0.5,subCurve[1].originBeginT);
	EXPECT_EQ(1.0,subCurve[1].originEndT);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//贝塞尔曲线被平行线分割.
//uni::Line = {points=[4]({x=10.000000 y=100.00000 },{x=82.420639 y=36.631943 },{x=48.307312 y=120.77245 },{x=174.72665 y=211.46967 }) }
//a = {originBeginT=0.00000000 originEndT=0.90525800 }
//uni::Line = {points=[2]({x=64.813370 y=93.689224 },{x=155.92406 y=154.22185 }) }
//uni::Line = {points=[2]({x=44.184212 y=124.73920 },{x=135.29491 y=185.27182 }) }
TEST(UGeometryTest,Bezier_clipByParallelLine_Works)
{
	CubicBezierLine cubic(10.0,100.0,82.420639,36.631943,48.307312,120.77245,174.72665,211.46967);
	StraightLine clip1(64.813370,93.689224,155.92406,154.22185);
	StraightLine clip2(44.184212,124.73920,135.29491,185.27182);
	std::vector<CubicBezierLine> subCurves = cubic.clipByParallelLine(clip1,clip2);
	EXPECT_EQ(2,subCurves.size());
	EXPECT_EQ(0.0,subCurves[0].originBeginT);
	EXPECT_EQ(1.0,subCurves[1].originEndT);
	if(HasNonfatalFailure())
	{
		FAIL();
	}
}

//>>浮点数比较.

//+0 == -0
TEST(UFloatTests,ZeroEqualsMinusZero)
{
	ASSERT_TRUE(AlmostEqualAbs(0.0,-0.0,1));
}