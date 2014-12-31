#include <catch.hpp>
#include <cmath>

#include "common/boundingbox.h"

TEST_CASE("Common::BoundingBox::BoundingBox()", "[boundingbox]") {
	auto A = Common::BoundingBox();

	CHECK(A.empty());
}

TEST_CASE("Common::BoundingBox::add(1,1,1)", "[boundingbox]") {
	auto A = Common::BoundingBox();

	A.add(1.0f, 1.0f, 1.0f);
	CHECK_FALSE(A.empty());
}

TEST_CASE("Common::BoundingBox::clear()", "[boundingbox]") {
	auto A = Common::BoundingBox();
	A.add(1.0f, 1.0f, 1.0f);

	A.clear();
	CHECK(A.empty());
}

TEST_CASE("Common::BoundingBox::isIn#1", "[boundingbox]") {
	auto A = Common::BoundingBox();

	A.add(0.0f, 0.0f, 0.0f);
	CHECK_FALSE(A.isIn(1.0f, 1.0f, 1.0f));

	A.add(2.0f, 2.0f, 2.0f);
	CHECK(A.isIn(1.0f, 1.0f, 1.0f));
}

TEST_CASE("Common::BoundingBox::isIn#2", "[boundingbox]") {
	auto A = Common::BoundingBox();

	A.add(-1.0f, -1.0f, -1.0f);
	CHECK_FALSE(A.isIn(1.0f, 1.0f, 1.0f));

	A.add(1.0f, 1.0f, 1.0f);
	A.rotate(45.0f, 0.0f, 0.0f, 1.0f);

	{
		float x, y, z;
		A.getMin(x, y, z);
		CHECK(x == -1.0f);
		CHECK(y == -1.0f);
		CHECK(z == -1.0f);
	}

	{
		float x, y, z;
		A.getMax(x, y, z);
		CHECK(x == 1.0f);
		CHECK(y == 1.0f);
		CHECK(z == 1.0f);
	}

	CHECK_FALSE(A.isIn(1.0f, 1.0f, 0.0f));
	CHECK(A.isIn(0.5f, 0.5f, 0.0f));
}

/*
class BoundingBox {
public:
	BoundingBox();
	~BoundingBox();

	void clear();

	bool empty() const;

	const TransformationMatrix &getOrigin() const;

	void getMin(float &x, float &y, float &z) const;
	void getMax(float &x, float &y, float &z) const;

	float getWidth () const; ///< Get the width of the bounding box.
	float getHeight() const; ///< Get the height of the bounding box.
	float getDepth () const; ///< Get the depth of the bounding box.

	bool isIn(float x, float y         ) const;
	bool isIn(float x, float y, float z) const;

	bool isIn(float x1, float y1, float z1, float x2, float y2, float z2) const;

	void add(float x, float y, float z);
	void add(const BoundingBox &box);

	void translate(float x, float y, float z);
	void scale    (float x, float y, float z);

	void rotate(float angle, float x, float y, float z);

	void transform(const TransformationMatrix &m);

	void absolutize();

	BoundingBox getAbsolute() const;
};
*/
