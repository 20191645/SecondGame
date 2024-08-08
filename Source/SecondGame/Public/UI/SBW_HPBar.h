// SBW_HPBar.h

#pragma once

#include "CoreMinimal.h"
#include "UI/SBW_Bar.h"
#include "SBW_HPBar.generated.h"

UCLASS()
class SECONDGAME_API USBW_HPBar : public USBW_Bar
{
	GENERATED_BODY()
	
public:
    // SetMaxfigure() 함수 호출
    void SetMaxHP(float InMaxHP);

    // 'BarWidget' 수치 초기화
    void InitializeHPBarWidget(class USStatComponent* NewStatComponent);

    // 'OnMaxHPChangedDelegate' 델리게이트에 바인드할 함수
    UFUNCTION()
    void OnMaxHPChange(float InOldMaxHP, float InNewMaxHP);
    // 'OnCurrentHPChangeDelegate' 델리게이트에 바인드할 함수
    UFUNCTION()
    void OnCurrentHPChange(float InOldHP, float InNewHP);

protected:
    virtual void NativeConstruct() override;
};
