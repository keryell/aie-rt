/******************************************************************************
*
* Copyright (C) 2005 - 2019 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMANGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
******************************************************************************/

/*****************************************************************************/
/**
* @file xaiegbl.h
* @{
*
* This file contains the instances of the register bit field definitions for the
* Core, Memory, NoC and PL module registers.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who     Date     Changes
* ----- ------  -------- -----------------------------------------------------
* 1.0   Tejus   09/24/2019  Initial creation
* 1.1   Tejus   10/28/2019  Add error type for pl interface apis
* 1.2   Tejus   12/09/2019  Forward declaration of structures
* 1.3   Tejus   03/17/2020  Add error types and data structure for lock apis
* 1.4   Tejus   03/21/2020  Add data strcuture and initialization function for
*			    packets.
* 1.5   Tejus   03/22/2020  Remove data strcutures from initial dma api
*			    implementation
* </pre>
*
******************************************************************************/

#ifndef XAIEGBL_H /* prevent circular inclusions */
#define XAIEGBL_H /* by using protection macros */

/***************************** Include Files *********************************/
#include "xaiegbl_defs.h"

/************************** Constant Definitions *****************************/
#define XAIE_LOCK_WITH_NO_VALUE	(-1)
/**************************** Type Definitions *******************************/
typedef struct XAie_TileMod XAie_TileMod;

/*
 * This typedef captures all the properties of a AIE Device
 */
typedef struct XAie_DevProp {
	u8 DevGen;
	u8 RowShift;
	u8 ColShift;
	XAie_TileMod *DevMod;
} XAie_DevProp;

/*
 * This typedef contains the attributes for a AIE partition. The structure is
 * setup during intialization.
 */
typedef struct {
	u64 BaseAddr; /* Base address of the partition*/
	u8 NumRows;   /* Number of rows allocated to the partition */
	u8 NumCols;   /* Number of cols allocated to the partition */
	u8 ShimRow;   /* ShimRow location */
	u8 MemTileRowStart; /* Mem tile starting row in the partition */
	u8 MemTileNumRows;  /* Number of memtile rows in the partition */
	u8 AieTileRowStart; /* Aie tile starting row in the partition */
	u8 AieTileNumRows;  /* Number of aie tile rows in the partition */
	u8 IsReady;
	XAie_DevProp DevProp; /* Pointer to the device property. To be
				     setup to AIE prop during intialization*/
} XAie_DevInst;

typedef struct {
	u8 AieGen;
	u64 BaseAddr;
	u8 ColShift;
	u8 RowShift;
	u8 NumRows;
	u8 NumCols;
	u8 ShimRowNum;
	u8 MemTileRowStart;
	u8 MemTileNumRows;
	u8 AieTileRowStart;
	u8 AieTileNumRows;
} XAie_Config;

/*
 * This typedef contains attributes for a tile coordinate.
 */
typedef struct {
	u8 Row;
	u8 Col;
} XAie_LocType;

/*
 * This typedef contains attributes for a range of AIE tiles.
 */
typedef struct {
	XAie_LocType Start;
	XAie_LocType End;
	XAie_LocType Stride;
} XAie_LocRange;

/*
 * This enum contains all the Stream Switch Port types. These enums are used to
 * access the base address of stream switch configuration registers.
 */
typedef enum{
	CORE,
	DMA,
	CTRL,
	FIFO,
	SOUTH,
	WEST,
	NORTH,
	EAST,
	TRACE,
	SS_PORT_TYPE_MAX
} StrmSwPortType;

/*
 * This enum contains the dma direction for aie dmas.
 */
typedef enum {
	DMA_S2MM,
	DMA_MM2S,
	DMA_MAX
} XAie_DmaDirection;

/*
 * This enum captures all the error codes from the driver
 */
typedef enum{
	XAIE_OK,
	XAIE_ERR,
	XAIE_INVALID_DEVICE,
	XAIE_INVALID_RANGE,
	XAIE_INVALID_ARGS,
	XAIE_INVALID_TILE,
	XAIE_ERR_STREAM_PORT,
	XAIE_INVALID_DMA_TILE,
	XAIE_INVALID_BD_NUM,
	XAIE_ERR_OUTOFBOUND,
	XAIE_INVALID_DATA_MEM_ADDR,
	XAIE_INVALID_ELF,
	XAIE_CORE_STATUS_TIMEOUT,
	XAIE_INVALID_CHANNEL_NUM,
	XAIE_INVALID_LOCK,
	XAIE_INVALID_DMA_DIRECTION,
	XAIE_INVALID_PLIF_WIDTH,
	XAIE_INVALID_LOCK_ID,
	XAIE_INVALID_LOCK_VALUE,
	XAIE_LOCK_RESULT_FAILED,
	XAIE_ERR_MAX
} AieRC;

/* Data structure to capture lock id and value */
typedef struct {
	u8 LockId;
	s8 LockVal;
} XAie_Lock;

/* Data structure to capture packet id and packet type */
typedef struct {
	u8 PktId;
	u8 PktType;
} XAie_Packet;

/**************************** Function prototypes ***************************/
AieRC XAie_CfgInitialize(XAie_DevInst *InstPtr, XAie_Config *ConfigPtr);

/*****************************************************************************/
/*
*
* This API returns a structure of type XAie_Loc which captures the lock id and
* lock value.
*
* @param	Id: Lock id
* @param	Value: Lock value.
*
* @return	Lock: Lock strcuture initialized with Id and Value.
*
* @note		None.
*
******************************************************************************/
static inline XAie_Lock XAie_LockInit(u8 Id, s8 Value)
{
	XAie_Lock Lock = {Id, Value};
	return Lock;
}

/*****************************************************************************/
/*
*
* This API returns a structure of type XAie_Packet which captures the packet id
* and packet type. XAie_Packet can be used to configure the packet properties
* of aie dmas and stream switches. Packet ID determins the route between ports,
* and packet type is used to differentiate packets from the same source.
*
* @param	PktId: Packet id(5 bits)
* @param	PktType: Packet type(3 bits)
*
* @return	Pkt: Packet strcuture initialized with Id and Type.
*
* @note		None.
*
******************************************************************************/
static inline XAie_Packet XAie_PacketInit(u8 PktId, u8 PktType)
{
	XAie_Packet Pkt = {PktId, PktType};
	return Pkt;
}

/*****************************************************************************/
/*
*
* This API returns a structure of type XAie_LocType given a col and row index of
* AIE. All APIs use this structure to identify the coordinates of AIE tiles.
*
* @param	col: column index
* @param	row: row index
*
* @return	Loc: strcuture containing row and col index.
*
* @note		None.
*
******************************************************************************/
static inline XAie_LocType XAie_TileLoc(u8 col, u8 row)
{
	XAie_LocType Loc = { row, col };
	return Loc;
}

/*****************************************************************************/
/*
*
* This API returns a structure of type XAie_LocRange which captures a range of
* AIE tiles.
*
* @param	Start: starting location of the range.
* @param	End: ending location of the range.
* @param	Stride: stride for row and column
*
* @return	Range: strcuture containing range of AIE Tiles.
*
* @note		None.
*
******************************************************************************/
static inline XAie_LocRange XAie_TileRange(XAie_LocType Start, XAie_LocType End,
		XAie_LocType Stride)
{
	XAie_LocRange Range = { Start, End, Stride };
	return Range;
}

/*****************************************************************************/
/**
*
* Macro to setup the configurate pointer data structure with hardware specific
* details.
*
* @param	Config: XAie_Config structure.
* @param	_AieGEn: Aie device generation.
* @param	_BaseAddr: Base Address of the device.
* @param	_ColShift: Bit shift value for column.
* @param	_RowShift: Bit shift value for row.
* @param	_NumCols: Number of cols in the hardware.
* @param	_NumRows: Number of rows in the hardware.
* @param	_ShimRowNum: Row number of the shimrow.
* @param	_MemTileRowStart: Starting row number of the mem tile.
* @param	_MemTileNumRows: Number of mem tile rows.
* @param	_AieTileRowStart: Starting row number of the mem tile.
* @param	_AieTileNumRows: Number of mem tile rows.
*
* @return	None.
*
* @note		The macro declares it XAie_Config as a stack variable.
*
*******************************************************************************/
#define XAie_SetupConfig(Config, _AieGen, _BaseAddr, _ColShift, _RowShift,\
		_NumCols, _NumRows, _ShimRowNum, _MemTileRowStart, _MemTileNumRows,\
		_AieTileRowStart, _AieTileNumRows) \
		XAie_Config Config = {\
			.AieGen = _AieGen,\
			.BaseAddr = _BaseAddr,\
			.ColShift = _ColShift,\
			.RowShift = _RowShift,\
			.NumRows = _NumRows,\
			.NumCols = _NumCols,\
			.ShimRowNum = _ShimRowNum,\
			.MemTileRowStart = _MemTileRowStart,\
			.MemTileNumRows = _MemTileNumRows,\
			.AieTileRowStart = _AieTileRowStart,\
			.AieTileNumRows = _AieTileNumRows,\
		}\

/*****************************************************************************/
/**
*
* Macro to declare device instance.
*
* @param	Inst: Name of the Device Instance variable.
* @param	ConfigPtr: Pointer to the XAie_Config structure containing the
*			   hardware details..
*
* @return	None.
*
* @note		The instance of a device must be always declared using this
*		macro. In future, the same macro will be expanded to allocate
*		more memory from the user application for resource management.
*
*******************************************************************************/
#define XAie_InstDeclare(Inst, ConfigPtr) XAie_DevInst Inst = { 0 }

#endif	/* end of protection macro */

/** @} */
