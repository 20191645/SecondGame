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
    // SetMaxfigure() �Լ� ȣ��
    void SetMaxHP(float InMaxHP);

    // 'BarWidget' ��ġ �ʱ�ȭ
    void InitializeHPBarWidget(class USStatComponent* NewStatComponent);

    // 'OnMaxHPChangedDelegate' ��������Ʈ�� ���ε��� �Լ�
    UFUNCTION()
    void OnMaxHPChange(float InOldMaxHP, float InNewMaxHP);
    // 'OnCurrentHPChangeDelegate' ��������Ʈ�� ���ε��� �Լ�
    UFUNCTION()
    void OnCurrentHPChange(float InOldHP, float InNewHP);

protected:
    virtual void NativeConstruct() override;
};
