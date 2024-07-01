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
    // 'OwningActor' Getter, Setter �Լ�
    AActor* GetOwningActor() const { return OwningActor; }
    void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

protected:
    // ���� ������ ���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USBaseWidget)
    TObjectPtr<AActor> OwningActor;
};
