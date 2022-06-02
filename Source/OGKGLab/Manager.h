// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Figure.h"
#include "GameFramework/Actor.h"
#include "SingleDot.h" 
#include "Manager.generated.h"
 
USTRUCT() 
struct  FTreeNode{

	GENERATED_BODY()
	
	bool bIsVertical = false;
	
	UPROPERTY()
	ASingleDot* value;

	FTreeNode* left;
	FTreeNode* right;
};

class btree{
public:
	btree();
	~btree();

	inline void preprocess(const TArray<ASingleDot*>& SortedYDots, const TArray<ASingleDot*>& SortedZDots);
	int32 pointsNumInRegion(AFigure* Figure);
	void checkNodeForPointsInRegion(int32 Y1, int32 Y2, int32 Z1, int32 Z2, AFigure* Figure, FTreeNode*& leaf, int32& res);
	void preprocessNode(const TArray<ASingleDot*>& SortedYDots, const TArray<ASingleDot*>& SortedZDots, FTreeNode*& leaf,  bool bIsVertical);
	 	void destroy_tree();
	 
private:
	void destroy_tree(FTreeNode *leaf);
 

	FTreeNode* root;
};


UCLASS()
class OGKGLAB_API AManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASingleDot*> Dots;
	
	UPROPERTY()
    TArray<ASingleDot*> SortedYDots;

	UPROPERTY()
    TArray<ASingleDot*> SortedZDots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFigure* Figure;
	
	btree Tree;

	UFUNCTION(BlueprintCallable)
	int32 GetPointNumInRegion();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
