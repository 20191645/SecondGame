// SGameResultWidget_Single.cpp

#include "UI/SGameResultWidget_Single.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

USGameResultWidget_Single::USGameResultWidget_Single(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USGameResultWidget_Single::NativeConstruct()
{
	// 버튼 클릭 이벤트와 멤버 함수 바인드
	ReturnButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void USGameResultWidget_Single::OnReturnButtonClicked()
{
	// 로딩 레벨로 이동
	// -- 로딩 레벨에서는 'NextLevel'을 파싱해서 'TitleLevel' 값을 얻어낸다
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=TitleLevel")));
}

void USGameResultWidget_Single::OnExitButtonClicked()
{
	// 게임 종료
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
