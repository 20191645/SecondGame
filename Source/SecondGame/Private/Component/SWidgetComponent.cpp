// SWidgetComponent.cpp

#include "Component/SWidgetComponent.h"
#include "UI/SBW_HPBar.h"

USWidgetComponent::USWidgetComponent()
{
}

void USWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// HP�� ������ ���� ���� ����
	USBW_HPBar* HPBarWidget = Cast<USBW_HPBar>(GetWidget());
	if (true == IsValid(HPBarWidget)) {
		HPBarWidget->SetOwningActor(GetOwner());
	}
}
