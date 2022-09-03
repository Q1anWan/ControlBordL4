/*
	DMA LowLayer库DMA支持
	  千万 2022.9.3
	  F1 F4 G0 G4 H7
	  Version:1.1
*/
#include "DMA_LL.h"
/*是否启用DMA*/
#if (defined(DMA1) || defined(DMA2) || defined(DMA3))&&(MY_DMA_ENABLE==1)

#ifndef LL_DMA_STREAM_1
void DMA_C::DMA_InitConfig(DMA_TypeDef *DMAx, uint32_t Channel)
{
	this->DMAx = DMAx;
	this->DMA_Channelx = Channel;
}

void DMA_C::DMA_EnableCH(void)
{
	LL_DMA_EnableChannel(this->DMAx, this->DMA_Channelx);
}
void DMA_C::DMA_DisableCH(void)
{
	LL_DMA_DisableChannel(this->DMAx, this->DMA_Channelx);
}

#if defined(DMA_IFCR_CTCIF1)
void DMA_C::DMA_Clear_TC(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_CHANNEL_1:
		LL_DMA_ClearFlag_TC1(this->DMAx);
		break;
	case LL_DMA_CHANNEL_2:
		LL_DMA_ClearFlag_TC2(this->DMAx);
		break;
	case LL_DMA_CHANNEL_3:
		LL_DMA_ClearFlag_TC3(this->DMAx);
		break;
	case LL_DMA_CHANNEL_4:
		LL_DMA_ClearFlag_TC4(this->DMAx);
		break;
#if defined(DMA1_Channel5)
	case LL_DMA_CHANNEL_5:
		LL_DMA_ClearFlag_TC5(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel6)
	case LL_DMA_CHANNEL_6:
		LL_DMA_ClearFlag_TC6(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel7)
	case LL_DMA_CHANNEL_7:
		LL_DMA_ClearFlag_TC7(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel8)
	case LL_DMA_CHANNEL_8:
		LL_DMA_ClearFlag_TC8(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_IFCR_CHTIF1)
void DMA_C::DMA_Clear_HT(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_CHANNEL_1:
		LL_DMA_ClearFlag_HT1(this->DMAx);
		break;
	case LL_DMA_CHANNEL_2:
		LL_DMA_ClearFlag_HT2(this->DMAx);
		break;
	case LL_DMA_CHANNEL_3:
		LL_DMA_ClearFlag_HT3(this->DMAx);
		break;
	case LL_DMA_CHANNEL_4:
		LL_DMA_ClearFlag_HT4(this->DMAx);
		break;
#if defined(DMA1_Channel5)
	case LL_DMA_CHANNEL_5:
		LL_DMA_ClearFlag_HT5(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel6)
	case LL_DMA_CHANNEL_6:
		LL_DMA_ClearFlag_HT6(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel7)
	case LL_DMA_CHANNEL_7:
		LL_DMA_ClearFlag_HT7(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel8)
	case LL_DMA_CHANNEL_8:
		LL_DMA_ClearFlag_HT8(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_IFCR_CGIF1)
void DMA_C::DMA_Clear_GI(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_CHANNEL_1:
		LL_DMA_ClearFlag_GI1(this->DMAx);
		break;
	case LL_DMA_CHANNEL_2:
		LL_DMA_ClearFlag_GI2(this->DMAx);
		break;
	case LL_DMA_CHANNEL_3:
		LL_DMA_ClearFlag_GI3(this->DMAx);
		break;
	case LL_DMA_CHANNEL_4:
		LL_DMA_ClearFlag_GI4(this->DMAx);
		break;
#if defined(DMA1_Channel5)
	case LL_DMA_CHANNEL_5:
		LL_DMA_ClearFlag_GI5(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel6)
	case LL_DMA_CHANNEL_6:
		LL_DMA_ClearFlag_GI6(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel7)
	case LL_DMA_CHANNEL_7:
		LL_DMA_ClearFlag_GI7(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel8)
	case LL_DMA_CHANNEL_8:
		LL_DMA_ClearFlag_GI8(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_IFCR_CTEIF1)
void DMA_C::DMA_Clear_TE(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_CHANNEL_1:
		LL_DMA_ClearFlag_TE1(this->DMAx);
		break;
	case LL_DMA_CHANNEL_2:
		LL_DMA_ClearFlag_TE2(this->DMAx);
		break;
	case LL_DMA_CHANNEL_3:
		LL_DMA_ClearFlag_TE3(this->DMAx);
		break;
	case LL_DMA_CHANNEL_4:
		LL_DMA_ClearFlag_TE4(this->DMAx);
		break;
#if defined(DMA1_Channel5)
	case LL_DMA_CHANNEL_5:
		LL_DMA_ClearFlag_TE5(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel6)
	case LL_DMA_CHANNEL_6:
		LL_DMA_ClearFlag_TE6(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel7)
	case LL_DMA_CHANNEL_7:
		LL_DMA_ClearFlag_TE7(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel8)
	case LL_DMA_CHANNEL_8:
		LL_DMA_ClearFlag_TE8(this->DMAx);
		break;
#endif
	}
}
#endif
#endif

#ifdef LL_DMA_STREAM_1

void DMA_C::DMA_InitConfig(DMA_TypeDef *DMAx, uint32_t Stream)
{
	this->DMAx = DMAx;
	this->DMA_Channelx = Stream;
}

void DMA_C::DMA_EnableCH(void)
{
	LL_DMA_EnableStream(this->DMAx, this->DMA_Channelx);
}

void DMA_C::DMA_DisableCH(void)
{
	LL_DMA_DisableStream(this->DMAx, this->DMA_Channelx);
}
#if defined(DMA_LIFCR_CTCIF1)
void DMA_C::DMA_Clear_TC(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_STREAM_0:
		LL_DMA_ClearFlag_TC0(this->DMAx);
		break;
	case LL_DMA_STREAM_1:
		LL_DMA_ClearFlag_TC1(this->DMAx);
		break;
	case LL_DMA_STREAM_2:
		LL_DMA_ClearFlag_TC2(this->DMAx);
		break;
	case LL_DMA_STREAM_3:
		LL_DMA_ClearFlag_TC3(this->DMAx);
		break;
#if defined(DMA1_Stream4)
	case LL_DMA_STREAM_4:
		LL_DMA_ClearFlag_TC4(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream5)
	case LL_DMA_STREAM_5:
		LL_DMA_ClearFlag_TC5(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream6)
	case LL_DMA_STREAM_6:
		LL_DMA_ClearFlag_TC6(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream7)
	case LL_DMA_STREAM_7:
		LL_DMA_ClearFlag_TC7(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_LIFCR_CHTIF1)
void DMA_C::DMA_Clear_HT(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_STREAM_0:
		LL_DMA_ClearFlag_HT0(this->DMAx);
		break;
	case LL_DMA_STREAM_1:
		LL_DMA_ClearFlag_HT1(this->DMAx);
		break;
	case LL_DMA_STREAM_2:
		LL_DMA_ClearFlag_HT2(this->DMAx);
		break;
	case LL_DMA_STREAM_3:
		LL_DMA_ClearFlag_HT3(this->DMAx);
		break;
#if defined(DMA1_Stream4)
	case LL_DMA_STREAM_4:
		LL_DMA_ClearFlag_HT4(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream5)
	case LL_DMA_STREAM_5:
		LL_DMA_ClearFlag_HT5(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream6)
	case LL_DMA_STREAM_6:
		LL_DMA_ClearFlag_HT6(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream7)
	case LL_DMA_STREAM_7:
		LL_DMA_ClearFlag_HT7(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_IFCR_CGIF1)
void DMA_C::DMA_Clear_GI(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_STREAM_0:
		LL_DMA_ClearFlag_GI0(this->DMAx);
		break;
	case LL_DMA_STREAM_1:
		LL_DMA_ClearFlag_GI1(this->DMAx);
		break;
	case LL_DMA_STREAM_2:
		LL_DMA_ClearFlag_GI2(this->DMAx);
		break;
	case LL_DMA_STREAM_3:
		LL_DMA_ClearFlag_GI3(this->DMAx);
		break;
#if defined(DMA1_Stream4)
	case LL_DMA_STREAM_4:
		LL_DMA_ClearFlag_GI4(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel5)
	case LL_DMA_STREAM_5:
		LL_DMA_ClearFlag_GI5(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel6)
	case LL_DMA_STREAM_6:
		LL_DMA_ClearFlag_GI6(this->DMAx);
		break;
#endif
#if defined(DMA1_Channel7)
	case LL_DMA_STREAM_7:
		LL_DMA_ClearFlag_GI7(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_IFCR_CTEIF1) || defined(DMA_LIFCR_CTEIF1)
void DMA_C::DMA_Clear_TE(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_STREAM_0:
		LL_DMA_ClearFlag_TE0(this->DMAx);
		break;
	case LL_DMA_STREAM_1:
		LL_DMA_ClearFlag_TE1(this->DMAx);
		break;
	case LL_DMA_STREAM_2:
		LL_DMA_ClearFlag_TE2(this->DMAx);
		break;
	case LL_DMA_STREAM_3:
		LL_DMA_ClearFlag_TE3(this->DMAx);
		break;
#if defined(DMA1_Stream4)
	case LL_DMA_STREAM_4:
		LL_DMA_ClearFlag_TE4(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream5)
	case LL_DMA_STREAM_5:
		LL_DMA_ClearFlag_TE5(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream6)
	case LL_DMA_STREAM_6:
		LL_DMA_ClearFlag_TE6(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream7)
	case LL_DMA_STREAM_7:
		LL_DMA_ClearFlag_TE7(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_LIFCR_CDMEIF1)
void DMA_C::DMA_Clear_DME(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_STREAM_0:
		LL_DMA_ClearFlag_DME0(this->DMAx);
		break;
	case LL_DMA_STREAM_1:
		LL_DMA_ClearFlag_DME1(this->DMAx);
		break;
	case LL_DMA_STREAM_2:
		LL_DMA_ClearFlag_DME2(this->DMAx);
		break;
	case LL_DMA_STREAM_3:
		LL_DMA_ClearFlag_DME3(this->DMAx);
		break;
#if defined(DMA1_Stream4)
	case LL_DMA_STREAM_4:
		LL_DMA_ClearFlag_DME4(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream5)
	case LL_DMA_STREAM_5:
		LL_DMA_ClearFlag_DME5(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream6)
	case LL_DMA_STREAM_6:
		LL_DMA_ClearFlag_DME6(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream7)
	case LL_DMA_STREAM_7:
		LL_DMA_ClearFlag_DME7(this->DMAx);
		break;
#endif
	}
}
#endif

#if defined(DMA_LIFCR_CFEIF1)
void DMA_C::DMA_Clear_FE(void)
{
	switch (this->DMA_Channelx)
	{
	case LL_DMA_STREAM_0:
		LL_DMA_ClearFlag_FE0(this->DMAx);
		break;
	case LL_DMA_STREAM_1:
		LL_DMA_ClearFlag_FE1(this->DMAx);
		break;
	case LL_DMA_STREAM_2:
		LL_DMA_ClearFlag_FE2(this->DMAx);
		break;
	case LL_DMA_STREAM_3:
		LL_DMA_ClearFlag_FE3(this->DMAx);
		break;
#if defined(DMA1_Stream4)
	case LL_DMA_STREAM_4:
		LL_DMA_ClearFlag_FE4(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream5)
	case LL_DMA_STREAM_5:
		LL_DMA_ClearFlag_FE5(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream6)
	case LL_DMA_STREAM_6:
		LL_DMA_ClearFlag_FE6(this->DMAx);
		break;
#endif
#if defined(DMA1_Stream7)
	case LL_DMA_STREAM_7:
		LL_DMA_ClearFlag_FE7(this->DMAx);
		break;
#endif
	}
}
#endif
#endif
#endif