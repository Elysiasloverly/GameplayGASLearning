// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Intercation/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/**
	 *来自游标的射线追踪，他有这些功能:
	 *A.最后一个演员是空的 && 当前演员是空的
	 *    -什么都不做
	 *B.最后一个演员是看空的 && 当前演员有效
	 *    -高光显示这个演员
	 *C.最后一个演员有效 && 当前演员是空的
	 *	  -不高光最后一个演员
	 *D.两个演员都是有效的，但指向的演员不相等
	 *	  -不高光最后一个演员，高光当前演员
	 *E.两个相等且有效
	 *	  -什么都不做
	 */
	
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//B
			ThisActor->HighlightActor();
		}
		else
		{
			//A
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			//C
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor!=ThisActor)
			{
				//D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//E
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeDate;
	InputModeDate.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeDate.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeDate);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}


