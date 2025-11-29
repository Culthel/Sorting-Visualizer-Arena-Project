#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SortingVisualizer.generated.h"

UENUM()
enum class EStepType : uint8
{
    Compare,
    Swap,
    Sorted
};

struct FSortStep
{
    EStepType Type;
    int32 IndexA;
    int32 IndexB;

    FSortStep(EStepType InType, int32 InA, int32 InB)
        : Type(InType), IndexA(InA), IndexB(InB) {
    }
};

UCLASS()
class MYPROJECT3_API ASortingVisualizer : public AActor
{
    GENERATED_BODY()

public:
    ASortingVisualizer();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Sorting Settings")
    int32 NumElements = 20;

    UPROPERTY(EditAnywhere, Category = "Sorting Settings")
    float StepDelay = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UStaticMesh* CubeMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UMaterialInterface* BaseMaterial;

    UPROPERTY(EditAnywhere, Category = "Visuals")

    USoundBase* SwapSound;

    UFUNCTION(BlueprintCallable)
    void GenerateArray();

    UFUNCTION(BlueprintCallable)
    void StartBubbleSort();

    UFUNCTION(BlueprintCallable, Category = "Sorting")
    void StartSelectionSort();

    UFUNCTION(BlueprintCallable)
    void StartInsertionSort();
   
    void CalculateInsertionSort();

private:
    TArray<int32> DataArray;
    TArray<UStaticMeshComponent*> BarMeshes;
    TArray<FSortStep> StepQueue;
    FTimerHandle SortTimerHandle;

    void CalculateBubbleSort();
    void CalculateSelectionSort();
    void ExecuteNextStep();
};