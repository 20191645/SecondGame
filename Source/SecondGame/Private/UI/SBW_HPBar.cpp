// SBW_HPBar.cpp

#include "UI/SBW_HPBar.h"
#include "Components/ProgressBar.h"
#include "Character/SCharacter.h"
#include "Component/SStatComponent.h"

void USBW_HPBar::SetMaxHP(float InMaxHP)
{
	// 'SBW_Bar' 클래스의 멤버 함수 호출
	SetMaxFigure(InMaxHP);
}

void USBW_HPBar::InitializeHPBarWidget(USStatComponent* NewStatComponent)
{
	// 초기화
	if (IsValid(NewStatComponent) == true)
	{
		OnCurrentHPChange(0.f, NewStatComponent->GetCurrentHP());
	}
}

void USBW_HPBar::OnMaxHPChange(float InOldMaxHP, float InNewMaxHP)
{
	SetMaxFigure(InNewMaxHP);

	OnCurrentHPChange(0.f, InNewMaxHP);
}

void USBW_HPBar::OnCurrentHPChange(float InOldHP, float InNewHP)
{
	// 'BarWidget'의 퍼센트 설정 (현재 HP / 최대 HP)
	if (IsValid(BarWidget) == true)
	{
		if (KINDA_SMALL_NUMBER < MaxFigure)
		{
			BarWidget->SetPercent(InNewHP / MaxFigure);
		}
		else
		{
			BarWidget->SetPercent(0.f);
		}
	}
}