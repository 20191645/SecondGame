// SBaseWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBaseWidget.generated.h"

UCLASS()
class SECONDGAME_API USBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 'OwningActor' Getter, Setter 함수
    AActor* GetOwningActor() const { return OwningActor; }
    void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

protected:
    // 위젯 소유한 액터 정보
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USBaseWidget)
    TObjectPtr<AActor> OwningActor;
};
