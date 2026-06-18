/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************//*!
 *
 * @file Ieee11073GetSize.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Agent Library Functions that returns size  
 *        of various objects
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <ieee11073_dimstruct.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif
#include <ieee11073_dimfuncproto.h>


/**************************************************************************//*!
 *
 * @name  GetAVA_Type
 *
 * @brief This function returns size of AVA_Type Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of AVA_Type Data Structure
 ******************************************************************************
 * This function returns size of AVA_Type Data Structure
 *****************************************************************************/
intu16 GetAVA_Type(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    AVA_Type *pAvaType = (AVA_Type*)pvPtr;
    intu16 size = (intu16)(sizeof(OID_Type) + ieee_ntohs(pAvaType->attribute_value.
        length) + ANY_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetOID_Type
 *
 * @brief This function returns size of OID_Type Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of OID_Type Data Structure
 ******************************************************************************
 * This function returns size of OID_Type Data Structure
 *****************************************************************************/
intu16 GetOID_Type(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    UNUSED(pvPtr)
    return (intu16)(sizeof(OID_Type));   
}

/**************************************************************************//*!
 *
 * @name  GetProdSpecEntry
 *
 * @brief This function returns size of ProdSpecEntry Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ProdSpecEntry Data Structure
 ******************************************************************************
 * This function returns size of ProdSpecEntry Data Structure
 *****************************************************************************/
intu16 GetProdSpecEntry(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ProdSpecEntry *pProdSpecEntry = (ProdSpecEntry*) pvPtr;
    intu16 size = (intu16)(sizeof(intu16) + sizeof(PrivateOid) + 
        pProdSpecEntry->prod_spec.length + OCTET_STRING_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetNuObsValue
 *
 * @brief This function returns size of NuObsValue Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of NuObsValue Data Structure
 ******************************************************************************
 * This function returns size of NuObsValue Data Structure
 *****************************************************************************/
intu16 GetNuObsValue(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    intu16 size = (intu16)(sizeof(OID_Type) + sizeof(MeasurementStatus) + 
        sizeof(OID_Type) + sizeof(FLOAT_Type));
    UNUSED(pvPtr)
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetAttrValMapEntry
 *
 * @brief This function returns size of AttrValMapEntry Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of AttrValMapEntry Data Structure
 ******************************************************************************
 * This function returns size of AttrValMapEntry Data Structure
 *****************************************************************************/
intu16 GetAttrValMapEntry(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    intu16 size = sizeof(OID_Type) + sizeof(intu16);
    UNUSED(pvPtr)
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetHandleAttrValMapEntry
 *
 * @brief This function returns size of HandleAttrValMapEntry Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of HandleAttrValMapEntry Data Structure
 ******************************************************************************
 * This function returns size of HandleAttrValMapEntry Data Structure
 *****************************************************************************/
intu16 GetHandleAttrValMapEntry(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    HandleAttrValMapEntry *pHandleAttrValMapEntry = (HandleAttrValMapEntry*)pvPtr;
    intu16 size = (intu16)(sizeof(HANDLE) + pHandleAttrValMapEntry->attr_val_map.length + 
        ATTR_VAL_MAP_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetInstNumber
 *
 * @brief This function returns size of InstNumber Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of InstNumber Data Structure
 ******************************************************************************
 * This function returns size of InstNumber Data Structure
 *****************************************************************************/
intu16 GetInstNumber(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    UNUSED(pvPtr)
    return (intu16)(sizeof(InstNumber));   
}

/**************************************************************************//*!
 *
 * @name  GetSegmentInfo
 *
 * @brief This function returns size of SegmentInfo Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of SegmentInfo Data Structure
 ******************************************************************************
 * This function returns size of SegmentInfo Data Structure
 *****************************************************************************/
intu16 GetSegmentInfo(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    SegmentInfo *pSegmentInfo = (SegmentInfo*)pvPtr;
    intu16 size = (intu16)(sizeof(InstNumber) + pSegmentInfo->seg_info.length + 
        ATTR_HEADER_SIZE);
    return size;
}

/**************************************************************************//*!
 *
 * @name  GetSegmEntryElem
 *
 * @brief This function returns size of SegmEntryElem Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of SegmEntryElem Data Structure
 ******************************************************************************
 * This function returns size of SegmEntryElem Data Structure
 *****************************************************************************/
intu16 GetSegmEntryElem(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    SegmEntryElem *pSegmEntryElem = (SegmEntryElem*) pvPtr;
    intu16 size = (intu16)(sizeof(OID_Type) + sizeof(HANDLE) + sizeof(TYPE) + 
        pSegmEntryElem->attr_val_map.length + ATTR_VAL_MAP_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetSegmElemStaticAttrEntry
 *
 * @brief This function returns size of SegmElemStaticAttrEntry Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of SegmElemStaticAttrEntry Data Structure
 ******************************************************************************
 * This function returns size of SegmEntryElem Data Structure
 *****************************************************************************/
intu16 GetSegmElemStaticAttrEntry(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    SegmElemStaticAttrEntry *pSegmElemStaticAttrEntry = 
        (SegmElemStaticAttrEntry*) pvPtr;
    intu16 size = (intu16)(sizeof(OID_Type) + GetTYPESize() + 
        pSegmElemStaticAttrEntry->attribute_list.length + ATTR_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetSegmentStatisticEntry
 *
 * @brief This function returns size of SegmentStatisticEntry Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of SegmentStatisticEntry Data Structure
 ******************************************************************************
 * This function returns size of SegmentStatisticEntry Data Structure
 *****************************************************************************/
intu16 GetSegmentStatisticEntry(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    SegmentStatisticEntry *pSegmentStatisticEntry = (SegmentStatisticEntry*) pvPtr;
    intu16 size = (intu16)(sizeof(SegmStatType) + pSegmentStatisticEntry->segm_stat_entry.length + 
        OCTET_STRING_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetDataProto
 *
 * @brief This function returns size of DataProto Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of DataProto Data Structure
 ******************************************************************************
 * This function returns size of DataProto Data Structure
 *****************************************************************************/
intu16 GetDataProto(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    DataProto *pDataProto = (DataProto*) pvPtr;
    intu16 size = (intu16)(sizeof(DataProtoId) + pDataProto->data_proto_info.length + 
        ANY_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetAttributeModEntry
 *
 * @brief This function returns size of AttributeModEntry Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of AttributeModEntry Data Structure
 ******************************************************************************
 * This function returns size of AttributeModEntry Data Structure
 *****************************************************************************/
intu16 GetAttributeModEntry(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    
    AttributeModEntry *pAttributeModEntry = (AttributeModEntry*) pvPtr;
    intu16 size = (intu16)(sizeof(ModifyOperator) + 
        GetAVA_Type((void*)&pAttributeModEntry->attribute));
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetTypeVer
 *
 * @brief This function returns size of TypeVer Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of TypeVer Data Structure
 ******************************************************************************
 * This function returns size of TypeVer Data Structure
 *****************************************************************************/
intu16 GetTypeVer(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    UNUSED(pvPtr)
    return (intu16)(sizeof(OID_Type) + sizeof(intu16));   
}

/**************************************************************************//*!
 *
 * @name  GetObservationScan
 *
 * @brief This function returns size of ObservationScan Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ObservationScan Data Structure
 ******************************************************************************
 * This function returns size of ObservationScan Data Structure
 *****************************************************************************/
intu16 GetObservationScan(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ObservationScan *pObservationScan = (ObservationScan*) pvPtr;
    intu16 size =  (intu16)(sizeof(HANDLE) + pObservationScan->attributes.length + 
        ATTR_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetScanReportPerVar
 *
 * @brief This function returns size of ScanReportPerVar Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ScanReportPerVar Data Structure
 ******************************************************************************
 * This function returns size of ScanReportPerVar Data Structure
 *****************************************************************************/
intu16 GetScanReportPerVar(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ScanReportPerVar *pScanReportPerVar = (ScanReportPerVar*) pvPtr;
    intu16 size = (intu16)(sizeof(intu16) + pScanReportPerVar->obs_scan_var.length +
        ATTR_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetObservationScanFixed
 *
 * @brief This function returns size of ObservationScanFixed Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ObservationScanFixed Data Structure
 ******************************************************************************
 * This function returns size of ObservationScanFixed Data Structure
 *****************************************************************************/
intu16 GetObservationScanFixed(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ObservationScanFixed *pObservationScanFixed = (ObservationScanFixed*) pvPtr;
    intu16 size = (intu16)(sizeof(HANDLE) + pObservationScanFixed->obs_val_data.length + 
        OCTET_STRING_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetScanReportPerFixed
 *
 * @brief This function returns size of ScanReportPerFixed Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ScanReportPerFixed Data Structure
 ******************************************************************************
 * This function returns size of ScanReportPerFixed Data Structure
 *****************************************************************************/
intu16 GetScanReportPerFixed(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ScanReportPerFixed *pScanReportPerFixed = (ScanReportPerFixed*) pvPtr;
    intu16 size = (intu16)(sizeof(intu16) + pScanReportPerFixed->obs_scan_fix.length + 
        ATTR_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetObservationScanGrouped
 *
 * @brief This function returns size of ObservationScanGrouped Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ObservationScanGrouped Data Structure
 ******************************************************************************
 * This function returns size of ObservationScanGrouped Data Structure
 *****************************************************************************/
intu16 GetObservationScanGrouped(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ObservationScanGrouped *pObservationScanGrouped = (ObservationScanGrouped*)pvPtr;
    intu16 size = (intu16)(pObservationScanGrouped->length + OCTET_STRING_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetScanReportPerGrouped
 *
 * @brief This function returns size of ScanReportPerGrouped Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ScanReportPerGrouped Data Structure
 ******************************************************************************
 * This function returns size of ScanReportPerGrouped Data Structure
 *****************************************************************************/
intu16 GetScanReportPerGrouped(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ScanReportPerGrouped *pScanReportPerGrouped = (ScanReportPerGrouped*)pvPtr;
    intu16 size = (intu16)(sizeof(PersonId) + pScanReportPerGrouped->obs_scan_grouped.length + 
        OCTET_STRING_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetConfigObject
 *
 * @brief This function returns size of ConfigObject Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of ConfigObject Data Structure
 ******************************************************************************
 * This function returns size of ConfigObject Data Structure
 *****************************************************************************/
intu16 GetConfigObject(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    ConfigObject *pConfigObject = (ConfigObject*)pvPtr;
    intu16 size = (intu16)(sizeof(OID_Type) + sizeof(HANDLE) + 
        pConfigObject->attributes.length + ATTR_HEADER_SIZE);
    return size;   
}

/**************************************************************************//*!
 *
 * @name  GetHANDLE
 *
 * @brief This function returns size of HANDLE Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of HANDLE Data Structure
 ******************************************************************************
 * This function returns size of HANDLE Data Structure
 *****************************************************************************/
intu16 GetHANDLE(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    UNUSED(pvPtr)
    return (intu16)(sizeof(HANDLE));   
}

/**************************************************************************//*!
 *
 * @name  GetSimpleNuObsValue
 *
 * @brief This function returns size of SimpleNuObsValue Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of SimpleNuObsValue Data Structure
 ******************************************************************************
 * This function returns size of SimpleNuObsValue Data Structure
 *****************************************************************************/
intu16 GetSimpleNuObsValue(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    UNUSED(pvPtr)
    return (intu16)(sizeof(FLOAT_Type));   
}

/**************************************************************************//*!
 *
 * @name  GetBasicNuObsValue
 *
 * @brief This function returns size of BasicNuObsValue Data Structure
 *
 * @param pvPtr	:	Pointer to Data Structure
 *
 * @return size of BasicNuObsValue Data Structure
 ******************************************************************************
 * This function returns size of BasicNuObsValue Data Structure
 *****************************************************************************/
intu16 GetBasicNuObsValue(
    void* pvPtr	/* [IN] Pointer to Data Structure */
)
{
    UNUSED(pvPtr)
    return (intu16)(sizeof(SFLOAT_Type));   
}
