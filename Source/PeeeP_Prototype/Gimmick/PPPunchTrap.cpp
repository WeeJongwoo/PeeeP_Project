// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPPunchTrap.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APPPunchTrap::APPPunchTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(SkeletalMesh);
	SkeletalMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	TriggerBox->SetupAttachment(SkeletalMesh, TEXT("Bone010Socket"));
	TriggerBox->bDynamicObstacle = true;
	TriggerBox->GetAttachParent();
	/*TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPPunchTrap::OnOverlapBegin);*/

	DetectStartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Detect"));
	DetectStartPosition->SetupAttachment(SkeletalMesh);
	DetectStartPosition->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // 400

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Gimmik/PunchTrap/ABP_PunchTrap.ABP_PunchTrap"));
	if (AnimInstanceClassRef.Class)
	{
		SkeletalMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//SkeletalMesh->SetAnimInstanceClass();
}

// Called when the game starts or when spawned
void APPPunchTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPPunchTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;

	FVector StartPos = DetectStartPosition->GetComponentLocation();
	FVector EndPos = StartPos + (DetectStartPosition->GetForwardVector() * 300.0f);
	FVector HitBoxHalfExtent = FVector(50.0f, 30.0f, 30.0f);

	FCollisionQueryParams CollisionParam(TEXT("Punch"), false, this);

	bool bIsHit = GetWorld()->SweepSingleByChannel(HitResult, StartPos, EndPos, FQuat::Identity, ECC_GameTraceChannel6, FCollisionShape::MakeBox(HitBoxHalfExtent), CollisionParam);
	if (bIsHit)
	{
		APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(HitResult.GetActor());
		if (Player)
		{
			if (!SkeletalMesh->GetAnimInstance()->Montage_IsPlaying(JabMontage))
			{
				SkeletalMesh->GetAnimInstance()->Montage_Play(JabMontage, 1.0f);
			}
		}
	}

	//FVector Detect
}

//void APPPunchTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	UE_LOG(LogTemp, Log, TEXT("Triggered!"));
//	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(OtherActor);
//	if (PlayerCharacter)
//	{
//		UE_LOG(LogTemp, Log, TEXT("Cast Completed!"));
//		PlayerCharacter->GetCharacterMovement()->AddImpulse(this->GetActorRightVector() * -500.0f, true);
//		PlayerCharacter->GetCharacterMovement()->AddImpulse(this->GetActorUpVector() * 300.0f, true);
//	}
//}
//