// SGameReulstWidget_Multi.cpp

#include "UI/SGameResultWidget_Multi.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USGameResultWidget_Multi::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트와 멤버 함수 바인드
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
