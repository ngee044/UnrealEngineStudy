// Fill out your copyright notice in the Description page of Project Settings.


#include "FireCameraShake.h"

UFireCameraShake::UFireCameraShake()
{
	OscillationDuration = 0.12f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = 5.0f;
	RotOscillation.Pitch.Frequency = 1;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}