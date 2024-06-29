// SPlayerController.cpp

#include "Controller/SPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// SetInputMode(): �÷��� ��ư ������ �ٷ� ��Ŀ�� �ǰԲ� ��
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if (IsValid(CrosshairUIClass) == true)
	{
		UUserWidget* CrosshairUIInstance = CreateWidget<UUserWidget>(this, CrosshairUIClass);
		if (IsValid(CrosshairUIInstance) == true)
		{
			// ������ ���� ȭ�鿡 �߰�
			CrosshairUIInstance->AddToViewport(1);
			CrosshairUIInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
