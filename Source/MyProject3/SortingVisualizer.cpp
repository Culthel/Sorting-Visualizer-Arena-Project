#include "SortingVisualizer.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASortingVisualizer::ASortingVisualizer()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ASortingVisualizer::BeginPlay()
{
    Super::BeginPlay();
}

void ASortingVisualizer::GenerateArray()
{
    for (UStaticMeshComponent* Mesh : BarMeshes)
    {
        if (Mesh) Mesh->DestroyComponent();
    }
    BarMeshes.Empty();
    DataArray.Empty();
    StepQueue.Empty();

    for (int32 i = 0; i < NumElements; i++)
    {
        int32 RandomValue = FMath::RandRange(10, 100);
        DataArray.Add(RandomValue);

        UStaticMeshComponent* NewBar = NewObject<UStaticMeshComponent>(this);
        NewBar->RegisterComponent();
        NewBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        if (CubeMesh) NewBar->SetStaticMesh(CubeMesh);
        if (BaseMaterial) NewBar->SetMaterial(0, BaseMaterial);

        FVector Location(i * 60.0f, 0, 0);
        NewBar->SetRelativeLocation(Location);
        NewBar->SetWorldScale3D(FVector(0.5f, 0.5f, RandomValue / 20.0f));

        BarMeshes.Add(NewBar);
    }
}

void ASortingVisualizer::StartBubbleSort()
{
    if (DataArray.Num() == 0) return;

    StepQueue.Empty();
    CalculateBubbleSort();

    GetWorld()->GetTimerManager().SetTimer(SortTimerHandle, this, &ASortingVisualizer::ExecuteNextStep, StepDelay, true);
}

void ASortingVisualizer::CalculateBubbleSort()
{
    TArray<int32> TempData = DataArray;
    int32 n = TempData.Num();

    for (int32 i = 0; i < n - 1; i++)
    {
        for (int32 j = 0; j < n - i - 1; j++)
        {
            StepQueue.Add(FSortStep(EStepType::Compare, j, j + 1));

            if (TempData[j] > TempData[j + 1])
            {
                int32 Temp = TempData[j];
                TempData[j] = TempData[j + 1];
                TempData[j + 1] = Temp;

                StepQueue.Add(FSortStep(EStepType::Swap, j, j + 1));
            }
        }
    }
}

void ASortingVisualizer::ExecuteNextStep()
{
    if (StepQueue.Num() == 0)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(SortTimerHandle);
        }
        return;
    }

    FSortStep CurrentStep = StepQueue[0];
    StepQueue.RemoveAt(0);

    if (CurrentStep.Type == EStepType::Swap)
    {
        if (!BarMeshes.IsValidIndex(CurrentStep.IndexA) || !BarMeshes.IsValidIndex(CurrentStep.IndexB))
        {
            return;
        }

        UStaticMeshComponent* BarA = BarMeshes[CurrentStep.IndexA];
        UStaticMeshComponent* BarB = BarMeshes[CurrentStep.IndexB];

        if (!BarA || !BarB) return;

        FVector ScaleA = BarA->GetRelativeScale3D();
        FVector ScaleB = BarB->GetRelativeScale3D();

        

        BarA->SetRelativeScale3D(FVector(ScaleA.X, ScaleA.Y, ScaleB.Z));
        BarB->SetRelativeScale3D(FVector(ScaleB.X, ScaleB.Y, ScaleA.Z));

        
      
            if (SwapSound)
            {
                UGameplayStatics::PlaySound2D(this, SwapSound);
            }
        
    }
}
void ASortingVisualizer::StartSelectionSort()
{
    if (DataArray.Num() == 0) return;

    StepQueue.Empty();
    CalculateSelectionSort(); 

    GetWorld()->GetTimerManager().SetTimer(SortTimerHandle, this, &ASortingVisualizer::ExecuteNextStep, StepDelay, true);
}

void ASortingVisualizer::CalculateSelectionSort()
{
    TArray<int32> TempData = DataArray;
    int32 n = TempData.Num();

    for (int32 i = 0; i < n - 1; i++)
    {
        int32 MinIndex = i;

        for (int32 j = i + 1; j < n; j++)
        {
            // Karþýlaþtýrma adýmýný kaydet
            StepQueue.Add(FSortStep(EStepType::Compare, MinIndex, j));

            if (TempData[j] < TempData[MinIndex])
            {
                MinIndex = j;
            }
        }

        // Eðer en küçük sayý baþtaki deðilse, takas et
        if (MinIndex != i)
        {
            int32 Temp = TempData[MinIndex];
            TempData[MinIndex] = TempData[i];
            TempData[i] = Temp;

            // Takas adýmýný kaydet
            StepQueue.Add(FSortStep(EStepType::Swap, i, MinIndex));
        }
    }
}

void ASortingVisualizer::StartInsertionSort()
{
    if (DataArray.Num() == 0) return;

    StepQueue.Empty();
    CalculateInsertionSort();

    GetWorld()->GetTimerManager().SetTimer(SortTimerHandle, this, &ASortingVisualizer::ExecuteNextStep, StepDelay, true);
}

void ASortingVisualizer::CalculateInsertionSort()
{
    TArray<int32> TempData = DataArray;
    int32 n = TempData.Num();

    for (int32 i = 1; i < n; i++)
    {
        int32 j = i;

        
        while (j > 0)
        {
            
            StepQueue.Add(FSortStep(EStepType::Compare, j, j - 1));

            if (TempData[j] < TempData[j - 1])
            {
                
                int32 Temp = TempData[j];
                TempData[j] = TempData[j - 1];
                TempData[j - 1] = Temp;

                
                StepQueue.Add(FSortStep(EStepType::Swap, j, j - 1));

                j--; 
            }
            else
            {
               
                break;
            }
        }
    }
}