// SPlayerController.cpp

#include "Controller/SPlayerController.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// SetInputMode(): �÷��� ��ư ������ �ٷ� ��Ŀ�� �ǰԲ� ��
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
