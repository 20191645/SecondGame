// SGameReulstWidget_Multi.cpp

#include "UI/SGameResultWidget_Multi.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USGameResultWidget_Multi::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư Ŭ�� �̺�Ʈ�� ��� �Լ� ���ε�
	ReturnButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void USGameResultWidget_Multi::OnReturnButtonClicked()
{
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=TitleLevel")));
}

void USGameResultWidget_Multi::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
