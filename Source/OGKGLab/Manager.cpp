// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager.h"
#include <algorithm>

#define INF 10000
 
struct Point
{
    int x;
    int y;
};
 
// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}
 
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);
 
    if (val == 0) return 0; // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 
// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}
 
// Returns true if the point p lies inside the polygon[] with n vertices
bool isInside(AFigure* Figure, int n, ASingleDot* dot)
{
	
	Point p = {dot->GetActorLocation().Y, dot->GetActorLocation().Z};
    // There must be at least 3 vertices in polygon[]
    if (n < 3) return false;
 
    // Create a point for line segment from p to infinite
    Point extreme = {INF, p.y};
 
    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i+1)%n;
		Point pi = {Figure->FigureDots[i]->GetActorLocation().Y,Figure->FigureDots[i]->GetActorLocation().Z};
    	Point pn = {Figure->FigureDots[next]->GetActorLocation().Y, Figure->FigureDots[next]->GetActorLocation().Z};
        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(pi, pn, p, extreme))
        {
            // If the point 'p' is collinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(pi, p, pn) == 0)
            return onSegment(pi, p, pn);
 
            count++;
        }
        i = next;
    } while (i != 0);
 
    // Return true if count is odd, false otherwise
    return count&1; // Same as (count%2 == 1)
}
// Sets default values
AManager::AManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AManager::BeginPlay()
{
	Super::BeginPlay();
	
	SortedYDots = Dots;
	SortedZDots = Dots;
	
	SortedYDots.Sort([]( const ASingleDot& FirstDot,  ASingleDot& SecondDot)
    {
        return FirstDot.GetActorLocation().Y < SecondDot.GetActorLocation().Y;
    });

	SortedZDots.Sort([](const ASingleDot& FirstDot, const ASingleDot& SecondDot)
    {
        return FirstDot.GetActorLocation().Z < SecondDot.GetActorLocation().Z;
    });

	Tree.preprocess(SortedYDots, SortedZDots);

 
}

int32 AManager::GetPointNumInRegion()
{
	return Tree.pointsNumInRegion(Figure); 
}

// Called every frame
void AManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


btree::btree(){
	root = NULL;
}

btree::~btree(){
	destroy_tree();
}

void btree::destroy_tree(FTreeNode *leaf){
	if(leaf != NULL){
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}
 
void btree::destroy_tree(){
	destroy_tree(root);
}
 
void btree::preprocess(const TArray<ASingleDot*>& SortedYDots,
    const TArray<ASingleDot*>& SortedZDots)
{
	destroy_tree();
	preprocessNode(SortedYDots, SortedZDots, root, true); 
}

void btree::preprocessNode(const TArray<ASingleDot*>& SortedYDots, const TArray<ASingleDot*>& SortedZDots, FTreeNode*& leaf,
    bool bIsVertical)
{
 
	leaf = new FTreeNode();
	leaf->bIsVertical = bIsVertical;
	if(bIsVertical)
	{
		leaf->value = SortedYDots[SortedYDots.Num()/2];
		TArray<ASingleDot*> LeftSortedY;
		TArray<ASingleDot*> RightSortedY;

		for(int32 i = 0; i < SortedYDots.Num()/2; ++i)
		{
			LeftSortedY.Add(SortedYDots[i]);
		}
		
		for(int32 i = SortedYDots.Num()/2 + 1; i < SortedYDots.Num(); ++i)
		{
			RightSortedY.Add(SortedYDots[i]);
		}

		
		TArray<ASingleDot*> LeftSortedZ = LeftSortedY;
		TArray<ASingleDot*> RightSortedZ = RightSortedY;
		
		LeftSortedZ.Sort([](const ASingleDot& FirstDot, const ASingleDot& SecondDot)
        {
            return FirstDot.GetActorLocation().Z < SecondDot.GetActorLocation().Z;
        });

		RightSortedZ.Sort([](const ASingleDot& FirstDot, const ASingleDot& SecondDot)
        {
            return FirstDot.GetActorLocation().Z < SecondDot.GetActorLocation().Z;
        });

		if(LeftSortedY.Num()>0)
		{
			preprocessNode(LeftSortedY, LeftSortedZ, leaf->left, !bIsVertical);	
		}
		if(RightSortedY.Num() > 0)
		{
			preprocessNode(RightSortedY, RightSortedZ, leaf->right, !bIsVertical);
		}
	}
	else
	{
		leaf->value = SortedZDots[SortedZDots.Num()/2]; 
		TArray<ASingleDot*> UpSortedZ;
		TArray<ASingleDot*> DownSortedZ;

		for(int32 i = 0; i < SortedZDots.Num()/2; ++i)
		{
			DownSortedZ.Add(SortedZDots[i]);
		}
		
		for(int32 i = SortedZDots.Num()/2 + 1; i < SortedZDots.Num(); ++i)
		{
			UpSortedZ.Add(SortedZDots[i]);
		}

		
		TArray<ASingleDot*> UpSortedY = UpSortedZ;
		TArray<ASingleDot*> DownSortedY = DownSortedZ;
		
		UpSortedY.Sort([](const ASingleDot& FirstDot, const ASingleDot& SecondDot)
        {
            return FirstDot.GetActorLocation().Y < SecondDot.GetActorLocation().Y;
        });

		DownSortedY.Sort([](const ASingleDot& FirstDot, const ASingleDot& SecondDot)
        {
            return FirstDot.GetActorLocation().Y < SecondDot.GetActorLocation().Y;
        });

		if(DownSortedY.Num() > 0)
		{
			preprocessNode(DownSortedY, DownSortedZ, leaf->left, !bIsVertical);	
		}
		if (UpSortedY.Num() > 0)
		{
			preprocessNode(UpSortedY, UpSortedZ, leaf->right, !bIsVertical);	
		}
	}
}

int32 btree::pointsNumInRegion(AFigure* Figure)
{
	int32 res = 0;
	int32 MinY = INT32_MAX;
	int32 MaxY = INT32_MIN;
	int32 MinZ = INT32_MAX;
	int32 MaxZ = INT32_MIN;

	for(int32 i = 0; i < Figure->FigureDots.Num(); i++)
	{
		int32 Y = Figure->FigureDots[i]->GetActorLocation().Y;
		int32 Z = Figure->FigureDots[i]->GetActorLocation().Z;
		
		if(Y > MaxY)
		{
			MaxY = Y;	
		}
		if(Y < MinY)
		{
			MinY = Y;
		}
		if(Z > MaxZ)
		{
			MaxZ = Z;
		}
		if(Z < MinZ)
		{
			MinZ = Z;
		}
	}
	
	checkNodeForPointsInRegion(MinY, MaxY, MinZ, MaxZ, Figure, root, res);
	return  res;
}

void btree::checkNodeForPointsInRegion(int32 Y1, int32 Y2, int32 Z1, int32 Z2, AFigure* Figure, FTreeNode*& leaf, int32& res)
{
	if(leaf)
	{
		int Y = leaf->value->GetActorLocation().Y;
		int Z = leaf->value->GetActorLocation().Z;
		UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), leaf->value->GetActorLocation().X,leaf->value->GetActorLocation().Y,leaf->value->GetActorLocation().Z);
		 
		if(leaf->bIsVertical)
		{
			if(Y <= Y2)
			{
				 checkNodeForPointsInRegion(Y1,Y2,Z1,Z2,Figure, leaf->right,res);
			}
			
			if(Y >= Y1)
			{
				 checkNodeForPointsInRegion(Y1,Y2,Z1,Z2, Figure, leaf->left,res);
			}

			if(Y <= Y2  && Y >= Y1 && Z >= Z1 && Z <= Z2)
			{ 
				if(isInside(Figure, Figure->FigureDots.Num(), leaf->value))
				{
					UE_LOG(LogTemp, Warning, TEXT("+1"));
					res +=1;
				}
				
			}
		}
		else
		{
			if(Z <= Z2)
			{
				checkNodeForPointsInRegion(Y1,Y2,Z1,Z2, Figure, leaf->right,res);
			}
			
			if(Z >= Z1)
			{
				 checkNodeForPointsInRegion(Y1,Y2,Z1,Z2, Figure, leaf->left,res);
			}

			if(Y <= Y2  && Y >= Y1 && Z >= Z1 && Z <= Z2)
			{ 
				if(isInside(Figure, Figure->FigureDots.Num(), leaf->value))
				{
			 
					res +=1;
				}
				
			}
		} 
	}
	else
	{ 
	}
}
