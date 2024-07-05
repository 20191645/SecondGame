// BTService_DetectPlayerCharacter.cpp

#include "AI/BTService_DetectPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/SPlayerCharacter.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Behavior Tree에 보일 노드 이름 설정
	NodeName = TEXT("DetectPlayerCharacter");
	// TickNode() 함수 호출 주기 설정
	Interval = 1.f;
}

void UBTService_DetectPlayerCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIC = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIC) == true)
	{
		ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIC->GetPawn());
		if (IsValid(NPC) == true)
		{
			UWorld* World = NPC->GetWorld();
			if (IsValid(World) == true)
			{
				FVector CenterPosition = NPC->GetActorLocation();
				// 플레이어 캐릭터 탐지 거리
				float DetectRadius = 1500.f;

				// 하나 이상의 오브젝트를 가져올거기 때문에 TArray 사용
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				// 주위 오브젝트들 감지 -- {Overlap}{Multi}{ByChannel}
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel3,	// = 'Detect' Trace Channel
					FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams
				);

				// 오브젝트가 감지된 경우, Player Character 가려내기
				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{
						ASPlayerCharacter* PC = Cast<ASPlayerCharacter>(OverlapResult.GetActor());
						// IsPlayerController(): 감지된 캐릭터가 플레이어가 조종하고 있는 폰인지 확인
						if (IsValid(PC) == true && PC->GetController()->IsPlayerController() == true)
						{
							// 플레이어 캐릭터 탐지 성공
							// 'BB_NPC'의 'TargetActor' Key 값을 'PlayerCharacter'로 설정
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, PC);
						}
					}
				}
				else
				{
					// 'BB_NPC'의 'TargetActor' Key 값을 nullptr로 설정
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);
				}
			}
		}
	}
}
