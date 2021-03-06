
#include "common.h"
#include "dma.h"
#include "tim.h"

/*****************************************************************************/
/* ���� ���������� ���������� ����� �������������� � ������� */
volatile uint8_t dma_end_transf;

/*****************************************************************************/

uint32_t dma_init(void){
	/* ���������� ������������ DMA1 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* ��������� ������� ������
           ������ �������� 16 ���,
           ���������� ���������� �� ����������,
           ������������ ��������� ������,
           ��������� ������ � ������ */
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE |
                          DMA_CCR1_PSIZE_0 |
                          DMA_CCR1_MSIZE_0 |
                          DMA_CCR1_PL |
                          DMA_CCR1_MINC;
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    /* ���������� ���������� �� ������ �� ���������� �������� */
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    /* ������������� ����� ���������� �������������� � ������� */
    dma_end_transf = 0;

    return (DMA_INIT_OK);
}

/*****************************************************************************/

void DMA1_Channel1_IRQHandler(void) {
	/* ���������� ���� ���������� �������� */
    dma_end_transf = 1;
    /* ��������� ������� �������� ��������� */
    tim3_stop();
    /* �������� ���������� �������� */
    if(DMA1->ISR & DMA_ISR_TCIF1){
    	/* ����� ����� ���������� */
        DMA1->IFCR  |= DMA_IFCR_CTCIF1;
    }
    /* ��������� ������ */
    DMA1_Channel1->CCR  &= ~DMA_CCR1_EN;
}

/*****************************************************************************/

void dma_ch_prep(uint32_t dma_cnt_b){

	/*uint32_t i=0;
	for(i=0; i<dma_cnt_b; i++) {
		adc_data_ar[i] = (uint16_t) 0x0000;
	}*/

	/* ����� ���������� � ������ */
    DMA1_Channel1->CMAR  = (uint32_t) &adc_data_ar;
    /* ����� ��������� ��������� */
    DMA1_Channel1->CPAR  = (uint32_t) &(ADC1->DR);
    /* ��������� ���������� ������������ ������ */
    DMA1_Channel1->CNDTR = dma_cnt_b;
    /* ���������� ������ ������ */
    DMA1_Channel1->CCR  |= DMA_CCR1_EN;

}

/*****************************************************************************/
