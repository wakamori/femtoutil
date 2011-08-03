#include <visual.hpp>
#include <del_interface.hpp>
//====================== Written by Takuma Wakamori =========================//

#ifdef __cplusplus
extern "C" {
#endif

//static vector<Triangle> triangulate(const vector<Vec2f> & points, float resolution = 50.0f);

#define fMIN(x, y) (((x) < (y)) ? x : y)
#define fMAX(x, y) (((x) < (y)) ? y : x)

std::vector<Triangle> triangulate(const std::vector<Vec2f> & points, float resolution = 50.0f)
{
	std::vector<Triangle> mTriangles;
	std::vector<Vec2f> mPoints = points;
	float mSize = mPoints.size();
	float mCount = fMIN(resolution, mSize);
	tpp::Delaunay::Point mPoint;
	std::vector<tpp::Delaunay::Point> mVertices;
	for (int32_t i = 0; i < mCount; i++) {
		int32_t mId = (int32_t)((float)i / mCount * mSize);
		mPoint[0] = mPoints[mId].x;
		mPoint[1] = mPoints[mId].y;
		mVertices.push_back(mPoint);
	}
	tpp::Delaunay mDelaunay(mVertices);
	mDelaunay.Triangulate();
	for (tpp::Delaunay::fIterator mTriIt = mDelaunay.fbegin(); mTriIt != mDelaunay.fend(); ++mTriIt) {
		int32_t mA = mDelaunay.Org(mTriIt);
		int32_t mB = mDelaunay.Dest(mTriIt);
		int32_t mC = mDelaunay.Apex(mTriIt);
		int32_t mAId = (int32_t)(((float)mA / resolution) * mSize);
		int32_t mBId = (int32_t)(((float)mB / resolution) * mSize);
		int32_t mCId = (int32_t)(((float)mC / resolution) * mSize);
		Vec2f mTriangle[3];
		mTriangle[0] = Vec2f(mPoints[mAId].x, mPoints[mAId].y);
		mTriangle[1] = Vec2f(mPoints[mBId].x, mPoints[mBId].y);
		mTriangle[2] = Vec2f(mPoints[mCId].x, mPoints[mCId].y);
		Vec2f mCentroid = Vec2f(
				(mTriangle[0].x + mTriangle[1].x + mTriangle[2].x) / 3.0f,
				(mTriangle[0].y + mTriangle[1].y + mTriangle[2].y) / 3.0f
			);
		int32_t mCounter = 0;
		Vec2f mPointA = mPoints[0];
		Vec2f mPointB;
		for (int32_t i = 1; i < (int32_t)mSize; i++) {
			mPointB = mPoints[i];
			if (mCentroid.y > fMIN(mPointA.y, mPointB.y) &&
					mCentroid.y <= fMAX(mPointA.y, mPointB.y) &&
					mCentroid.x <= fMAX(mPointA.x, mPointB.x) &&
					mPointA.y != mPointB.y &&
					(mPointA.x == mPointB.x || mCentroid.x <= (mCentroid.y - mPointA.y) * (mPointB.x - mPointA.x) / (mPointB.y - mPointA.y) + mPointA.x))
				mCounter++;
			mPointA = mPointB;
		}
		if (mCounter % 2 != 0) {
			Triangle mTriData;
			float x0 = mTriangle[0].x;
			float y0 = mTriangle[0].y;
			float x1 = mTriangle[1].x;
			float y1 = mTriangle[1].y;
			float x2 = mTriangle[2].x;
			float y2 = mTriangle[2].y;
			if (((x1 - x0) * (y2 - y0) - (x2 -x0) * (y1 - y0)) > 0) {
				mTriData.a = Vec2f(x0, y0);
				mTriData.b = Vec2f(x1, y1);
				mTriData.c = Vec2f(x2, y2);
			} else {
				mTriData.a = Vec2f(x0, y0);
				mTriData.b = Vec2f(x2, y2);
				mTriData.c = Vec2f(x1, y1);
			}
			mTriangles.push_back(mTriData);
		}
	}
	return mTriangles;
}

#ifdef __cplusplus
}
#endif
