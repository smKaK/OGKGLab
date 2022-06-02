// Fill out your copyright notice in the Description page of Project Settings.


#include "Figure.h"

// Sets default values
AFigure::AFigure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFigure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFigure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

