/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SBE_NEWORDERSINGLE_CXX_H_
#define _SBE_NEWORDERSINGLE_CXX_H_

#if __cplusplus >= 201103L
#define SBE_CONSTEXPR constexpr
#define SBE_NOEXCEPT noexcept
#else
#define SBE_CONSTEXPR
#define SBE_NOEXCEPT
#endif

#if __cplusplus >= 201703L
#include <string_view>
#define SBE_NODISCARD [[nodiscard]]
#if !defined(SBE_USE_STRING_VIEW)
#define SBE_USE_STRING_VIEW 1
#endif
#else
#define SBE_NODISCARD
#endif

#if __cplusplus >= 202002L
#include <span>
#if !defined(SBE_USE_SPAN)
#define SBE_USE_SPAN 1
#endif
#endif

#if !defined(__STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS 1
#endif

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#if defined(WIN32) || defined(_WIN32)
#define SBE_BIG_ENDIAN_ENCODE_16(v) _byteswap_ushort(v)
#define SBE_BIG_ENDIAN_ENCODE_32(v) _byteswap_ulong(v)
#define SBE_BIG_ENDIAN_ENCODE_64(v) _byteswap_uint64(v)
#define SBE_LITTLE_ENDIAN_ENCODE_16(v) (v)
#define SBE_LITTLE_ENDIAN_ENCODE_32(v) (v)
#define SBE_LITTLE_ENDIAN_ENCODE_64(v) (v)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define SBE_BIG_ENDIAN_ENCODE_16(v) __builtin_bswap16(v)
#define SBE_BIG_ENDIAN_ENCODE_32(v) __builtin_bswap32(v)
#define SBE_BIG_ENDIAN_ENCODE_64(v) __builtin_bswap64(v)
#define SBE_LITTLE_ENDIAN_ENCODE_16(v) (v)
#define SBE_LITTLE_ENDIAN_ENCODE_32(v) (v)
#define SBE_LITTLE_ENDIAN_ENCODE_64(v) (v)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define SBE_LITTLE_ENDIAN_ENCODE_16(v) __builtin_bswap16(v)
#define SBE_LITTLE_ENDIAN_ENCODE_32(v) __builtin_bswap32(v)
#define SBE_LITTLE_ENDIAN_ENCODE_64(v) __builtin_bswap64(v)
#define SBE_BIG_ENDIAN_ENCODE_16(v) (v)
#define SBE_BIG_ENDIAN_ENCODE_32(v) (v)
#define SBE_BIG_ENDIAN_ENCODE_64(v) (v)
#else
#error \
    "Byte Ordering of platform not determined. Set __BYTE_ORDER__ manually before including this file."
#endif

#if !defined(SBE_BOUNDS_CHECK_EXPECT)
#if defined(SBE_NO_BOUNDS_CHECK)
#define SBE_BOUNDS_CHECK_EXPECT(exp, c) (false)
#elif defined(_MSC_VER)
#define SBE_BOUNDS_CHECK_EXPECT(exp, c) (exp)
#else
#define SBE_BOUNDS_CHECK_EXPECT(exp, c) (__builtin_expect(exp, c))
#endif

#endif

#define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#define SBE_NULLVALUE_INT8 (std::numeric_limits<std::int8_t>::min)()
#define SBE_NULLVALUE_INT16 (std::numeric_limits<std::int16_t>::min)()
#define SBE_NULLVALUE_INT32 (std::numeric_limits<std::int32_t>::min)()
#define SBE_NULLVALUE_INT64 (std::numeric_limits<std::int64_t>::min)()
#define SBE_NULLVALUE_UINT8 (std::numeric_limits<std::uint8_t>::max)()
#define SBE_NULLVALUE_UINT16 (std::numeric_limits<std::uint16_t>::max)()
#define SBE_NULLVALUE_UINT32 (std::numeric_limits<std::uint32_t>::max)()
#define SBE_NULLVALUE_UINT64 (std::numeric_limits<std::uint64_t>::max)()

#include "DecimalEncoding.h"
#include "MessageHeader.h"
#include "QtyEncoding.h"
#include "SideEnum.h"

namespace sbe {

class NewOrderSingle {
 private:
  char *m_buffer = nullptr;
  std::uint64_t m_bufferLength = 0;
  std::uint64_t m_offset = 0;
  std::uint64_t m_position = 0;
  std::uint64_t m_actingBlockLength = 0;
  std::uint64_t m_actingVersion = 0;

  inline std::uint64_t *sbePositionPtr() SBE_NOEXCEPT { return &m_position; }

 public:
  static constexpr std::uint16_t SBE_BLOCK_LENGTH =
      static_cast<std::uint16_t>(46);
  static constexpr std::uint16_t SBE_TEMPLATE_ID =
      static_cast<std::uint16_t>(99);
  static constexpr std::uint16_t SBE_SCHEMA_ID = static_cast<std::uint16_t>(1);
  static constexpr std::uint16_t SBE_SCHEMA_VERSION =
      static_cast<std::uint16_t>(0);
  static constexpr const char *SBE_SEMANTIC_VERSION = "5.2";

  enum MetaAttribute { EPOCH, TIME_UNIT, SEMANTIC_TYPE, PRESENCE };

  union sbe_float_as_uint_u {
    float fp_value;
    std::uint32_t uint_value;
  };

  union sbe_double_as_uint_u {
    double fp_value;
    std::uint64_t uint_value;
  };

  using messageHeader = MessageHeader;

  NewOrderSingle() = default;

  NewOrderSingle(char *buffer, const std::uint64_t offset,
                 const std::uint64_t bufferLength,
                 const std::uint64_t actingBlockLength,
                 const std::uint64_t actingVersion)
      : m_buffer(buffer),
        m_bufferLength(bufferLength),
        m_offset(offset),
        m_position(sbeCheckPosition(offset + actingBlockLength)),
        m_actingBlockLength(actingBlockLength),
        m_actingVersion(actingVersion) {}

  NewOrderSingle(char *buffer, const std::uint64_t bufferLength)
      : NewOrderSingle(buffer, 0, bufferLength, sbeBlockLength(),
                       sbeSchemaVersion()) {}

  NewOrderSingle(char *buffer, const std::uint64_t bufferLength,
                 const std::uint64_t actingBlockLength,
                 const std::uint64_t actingVersion)
      : NewOrderSingle(buffer, 0, bufferLength, actingBlockLength,
                       actingVersion) {}

  SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeBlockLength()
      SBE_NOEXCEPT {
    return static_cast<std::uint16_t>(46);
  }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t sbeBlockAndHeaderLength()
      SBE_NOEXCEPT {
    return messageHeader::encodedLength() + sbeBlockLength();
  }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeTemplateId()
      SBE_NOEXCEPT {
    return static_cast<std::uint16_t>(99);
  }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeSchemaId() SBE_NOEXCEPT {
    return static_cast<std::uint16_t>(1);
  }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeSchemaVersion()
      SBE_NOEXCEPT {
    return static_cast<std::uint16_t>(0);
  }

  SBE_NODISCARD static const char *sbeSemanticVersion() SBE_NOEXCEPT {
    return "5.2";
  }

  SBE_NODISCARD static SBE_CONSTEXPR const char *sbeSemanticType()
      SBE_NOEXCEPT {
    return "D";
  }

  SBE_NODISCARD std::uint64_t offset() const SBE_NOEXCEPT { return m_offset; }

  NewOrderSingle &wrapForEncode(char *buffer, const std::uint64_t offset,
                                const std::uint64_t bufferLength) {
    m_buffer = buffer;
    m_bufferLength = bufferLength;
    m_offset = offset;
    m_actingBlockLength = sbeBlockLength();
    m_actingVersion = sbeSchemaVersion();
    m_position = sbeCheckPosition(m_offset + m_actingBlockLength);
    return *this;
  }

  NewOrderSingle &wrapAndApplyHeader(char *buffer, const std::uint64_t offset,
                                     const std::uint64_t bufferLength) {
    messageHeader hdr(buffer, offset, bufferLength, sbeSchemaVersion());

    hdr.blockLength(sbeBlockLength())
        .templateId(sbeTemplateId())
        .schemaId(sbeSchemaId())
        .version(sbeSchemaVersion());

    m_buffer = buffer;
    m_bufferLength = bufferLength;
    m_offset = offset + messageHeader::encodedLength();
    m_actingBlockLength = sbeBlockLength();
    m_actingVersion = sbeSchemaVersion();
    m_position = sbeCheckPosition(m_offset + m_actingBlockLength);
    return *this;
  }

  NewOrderSingle &wrapForDecode(char *buffer, const std::uint64_t offset,
                                const std::uint64_t actingBlockLength,
                                const std::uint64_t actingVersion,
                                const std::uint64_t bufferLength) {
    m_buffer = buffer;
    m_bufferLength = bufferLength;
    m_offset = offset;
    m_actingBlockLength = actingBlockLength;
    m_actingVersion = actingVersion;
    m_position = sbeCheckPosition(m_offset + m_actingBlockLength);
    return *this;
  }

  NewOrderSingle &sbeRewind() {
    return wrapForDecode(m_buffer, m_offset, m_actingBlockLength,
                         m_actingVersion, m_bufferLength);
  }

  SBE_NODISCARD std::uint64_t sbePosition() const SBE_NOEXCEPT {
    return m_position;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::uint64_t sbeCheckPosition(const std::uint64_t position) {
    if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false)) {
      throw std::runtime_error("buffer too short [E100]");
    }
    return position;
  }

  void sbePosition(const std::uint64_t position) {
    m_position = sbeCheckPosition(position);
  }

  SBE_NODISCARD std::uint64_t encodedLength() const SBE_NOEXCEPT {
    return sbePosition() - m_offset;
  }

  SBE_NODISCARD std::uint64_t decodeLength() const {
    NewOrderSingle skipper(m_buffer, m_offset, m_bufferLength,
                           m_actingBlockLength, m_actingVersion);
    skipper.skip();
    return skipper.encodedLength();
  }

  SBE_NODISCARD const char *buffer() const SBE_NOEXCEPT { return m_buffer; }

  SBE_NODISCARD char *buffer() SBE_NOEXCEPT { return m_buffer; }

  SBE_NODISCARD std::uint64_t bufferLength() const SBE_NOEXCEPT {
    return m_bufferLength;
  }

  SBE_NODISCARD std::uint64_t actingVersion() const SBE_NOEXCEPT {
    return m_actingVersion;
  }

  SBE_NODISCARD static const char *ClOrdIdMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "String";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t clOrdIdId() SBE_NOEXCEPT { return 11; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t clOrdIdSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool clOrdIdInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t clOrdIdEncodingOffset()
      SBE_NOEXCEPT {
    return 0;
  }

  static SBE_CONSTEXPR std::uint64_t clOrdIdNullValue() SBE_NOEXCEPT {
    return SBE_NULLVALUE_UINT64;
  }

  static SBE_CONSTEXPR std::uint64_t clOrdIdMinValue() SBE_NOEXCEPT {
    return UINT64_C(0x0);
  }

  static SBE_CONSTEXPR std::uint64_t clOrdIdMaxValue() SBE_NOEXCEPT {
    return UINT64_C(0xfffffffffffffffe);
  }

  static SBE_CONSTEXPR std::size_t clOrdIdEncodingLength() SBE_NOEXCEPT {
    return 8;
  }

  SBE_NODISCARD std::uint64_t clOrdId() const SBE_NOEXCEPT {
    std::uint64_t val;
    std::memcpy(&val, m_buffer + m_offset + 0, sizeof(std::uint64_t));
    return SBE_LITTLE_ENDIAN_ENCODE_64(val);
  }

  NewOrderSingle &clOrdId(const std::uint64_t value) SBE_NOEXCEPT {
    std::uint64_t val = SBE_LITTLE_ENDIAN_ENCODE_64(value);
    std::memcpy(m_buffer + m_offset + 0, &val, sizeof(std::uint64_t));
    return *this;
  }

  SBE_NODISCARD static const char *AccountMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "String";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t accountId() SBE_NOEXCEPT { return 1; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t accountSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool accountInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t accountEncodingOffset()
      SBE_NOEXCEPT {
    return 8;
  }

  static SBE_CONSTEXPR std::uint64_t accountNullValue() SBE_NOEXCEPT {
    return SBE_NULLVALUE_UINT64;
  }

  static SBE_CONSTEXPR std::uint64_t accountMinValue() SBE_NOEXCEPT {
    return UINT64_C(0x0);
  }

  static SBE_CONSTEXPR std::uint64_t accountMaxValue() SBE_NOEXCEPT {
    return UINT64_C(0xfffffffffffffffe);
  }

  static SBE_CONSTEXPR std::size_t accountEncodingLength() SBE_NOEXCEPT {
    return 8;
  }

  SBE_NODISCARD std::uint64_t account() const SBE_NOEXCEPT {
    std::uint64_t val;
    std::memcpy(&val, m_buffer + m_offset + 8, sizeof(std::uint64_t));
    return SBE_LITTLE_ENDIAN_ENCODE_64(val);
  }

  NewOrderSingle &account(const std::uint64_t value) SBE_NOEXCEPT {
    std::uint64_t val = SBE_LITTLE_ENDIAN_ENCODE_64(value);
    std::memcpy(m_buffer + m_offset + 8, &val, sizeof(std::uint64_t));
    return *this;
  }

  SBE_NODISCARD static const char *SymbolMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "String";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t symbolId() SBE_NOEXCEPT { return 55; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t symbolSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool symbolInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t symbolEncodingOffset()
      SBE_NOEXCEPT {
    return 16;
  }

  static SBE_CONSTEXPR std::uint64_t symbolNullValue() SBE_NOEXCEPT {
    return SBE_NULLVALUE_UINT64;
  }

  static SBE_CONSTEXPR std::uint64_t symbolMinValue() SBE_NOEXCEPT {
    return UINT64_C(0x0);
  }

  static SBE_CONSTEXPR std::uint64_t symbolMaxValue() SBE_NOEXCEPT {
    return UINT64_C(0xfffffffffffffffe);
  }

  static SBE_CONSTEXPR std::size_t symbolEncodingLength() SBE_NOEXCEPT {
    return 8;
  }

  SBE_NODISCARD std::uint64_t symbol() const SBE_NOEXCEPT {
    std::uint64_t val;
    std::memcpy(&val, m_buffer + m_offset + 16, sizeof(std::uint64_t));
    return SBE_LITTLE_ENDIAN_ENCODE_64(val);
  }

  NewOrderSingle &symbol(const std::uint64_t value) SBE_NOEXCEPT {
    std::uint64_t val = SBE_LITTLE_ENDIAN_ENCODE_64(value);
    std::memcpy(m_buffer + m_offset + 16, &val, sizeof(std::uint64_t));
    return *this;
  }

  SBE_NODISCARD static const char *SideMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "char";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t sideId() SBE_NOEXCEPT { return 54; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t sideSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool sideInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t sideEncodingOffset()
      SBE_NOEXCEPT {
    return 24;
  }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t sideEncodingLength()
      SBE_NOEXCEPT {
    return 1;
  }

  SBE_NODISCARD char sideRaw() const SBE_NOEXCEPT {
    char val;
    std::memcpy(&val, m_buffer + m_offset + 24, sizeof(char));
    return (val);
  }

  SBE_NODISCARD SideEnum::Value side() const {
    char val;
    std::memcpy(&val, m_buffer + m_offset + 24, sizeof(char));
    return SideEnum::get((val));
  }

  NewOrderSingle &side(const SideEnum::Value value) SBE_NOEXCEPT {
    char val = (value);
    std::memcpy(m_buffer + m_offset + 24, &val, sizeof(char));
    return *this;
  }

  SBE_NODISCARD static const char *TransactTimeMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "UTCTimestamp";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t transactTimeId() SBE_NOEXCEPT {
    return 60;
  }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t transactTimeSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool transactTimeInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t transactTimeEncodingOffset()
      SBE_NOEXCEPT {
    return 25;
  }

  static SBE_CONSTEXPR std::uint64_t transactTimeNullValue() SBE_NOEXCEPT {
    return SBE_NULLVALUE_UINT64;
  }

  static SBE_CONSTEXPR std::uint64_t transactTimeMinValue() SBE_NOEXCEPT {
    return UINT64_C(0x0);
  }

  static SBE_CONSTEXPR std::uint64_t transactTimeMaxValue() SBE_NOEXCEPT {
    return UINT64_C(0xfffffffffffffffe);
  }

  static SBE_CONSTEXPR std::size_t transactTimeEncodingLength() SBE_NOEXCEPT {
    return 8;
  }

  SBE_NODISCARD std::uint64_t transactTime() const SBE_NOEXCEPT {
    std::uint64_t val;
    std::memcpy(&val, m_buffer + m_offset + 25, sizeof(std::uint64_t));
    return SBE_LITTLE_ENDIAN_ENCODE_64(val);
  }

  NewOrderSingle &transactTime(const std::uint64_t value) SBE_NOEXCEPT {
    std::uint64_t val = SBE_LITTLE_ENDIAN_ENCODE_64(value);
    std::memcpy(m_buffer + m_offset + 25, &val, sizeof(std::uint64_t));
    return *this;
  }

  SBE_NODISCARD static const char *OrderQtyMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "Qty";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t orderQtyId() SBE_NOEXCEPT { return 38; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t orderQtySinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool orderQtyInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t orderQtyEncodingOffset()
      SBE_NOEXCEPT {
    return 33;
  }

 private:
  QtyEncoding m_orderQty;

 public:
  SBE_NODISCARD QtyEncoding &orderQty() {
    m_orderQty.wrap(m_buffer, m_offset + 33, m_actingVersion, m_bufferLength);
    return m_orderQty;
  }

  SBE_NODISCARD static const char *OrdTypeMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "char";
      case MetaAttribute::PRESENCE:
        return "required";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t ordTypeId() SBE_NOEXCEPT { return 40; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t ordTypeSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool ordTypeInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t ordTypeEncodingOffset()
      SBE_NOEXCEPT {
    return 37;
  }

  static SBE_CONSTEXPR std::uint8_t ordTypeNullValue() SBE_NOEXCEPT {
    return SBE_NULLVALUE_UINT8;
  }

  static SBE_CONSTEXPR std::uint8_t ordTypeMinValue() SBE_NOEXCEPT {
    return static_cast<std::uint8_t>(0);
  }

  static SBE_CONSTEXPR std::uint8_t ordTypeMaxValue() SBE_NOEXCEPT {
    return static_cast<std::uint8_t>(254);
  }

  static SBE_CONSTEXPR std::size_t ordTypeEncodingLength() SBE_NOEXCEPT {
    return 1;
  }

  SBE_NODISCARD std::uint8_t ordType() const SBE_NOEXCEPT {
    std::uint8_t val;
    std::memcpy(&val, m_buffer + m_offset + 37, sizeof(std::uint8_t));
    return (val);
  }

  NewOrderSingle &ordType(const std::uint8_t value) SBE_NOEXCEPT {
    std::uint8_t val = (value);
    std::memcpy(m_buffer + m_offset + 37, &val, sizeof(std::uint8_t));
    return *this;
  }

  SBE_NODISCARD static const char *PriceMetaAttribute(
      const MetaAttribute metaAttribute) SBE_NOEXCEPT {
    switch (metaAttribute) {
      case MetaAttribute::SEMANTIC_TYPE:
        return "Price";
      case MetaAttribute::PRESENCE:
        return "optional";
      default:
        return "";
    }
  }

  static SBE_CONSTEXPR std::uint16_t priceId() SBE_NOEXCEPT { return 44; }

  SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t priceSinceVersion()
      SBE_NOEXCEPT {
    return 0;
  }

  SBE_NODISCARD bool priceInActingVersion() SBE_NOEXCEPT { return true; }

  SBE_NODISCARD static SBE_CONSTEXPR std::size_t priceEncodingOffset()
      SBE_NOEXCEPT {
    return 38;
  }

 private:
  DecimalEncoding m_price;

 public:
  SBE_NODISCARD DecimalEncoding &price() {
    m_price.wrap(m_buffer, m_offset + 38, m_actingVersion, m_bufferLength);
    return m_price;
  }

  template <typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits> &operator<<(
      std::basic_ostream<CharT, Traits> &builder,
      const NewOrderSingle &_writer) {
    NewOrderSingle writer(_writer.m_buffer, _writer.m_offset,
                          _writer.m_bufferLength, _writer.m_actingBlockLength,
                          _writer.m_actingVersion);

    builder << '{';
    builder << R"("Name": "NewOrderSingle", )";
    builder << R"("sbeTemplateId": )";
    builder << writer.sbeTemplateId();
    builder << ", ";

    builder << R"("ClOrdId": )";
    builder << +writer.clOrdId();

    builder << ", ";
    builder << R"("Account": )";
    builder << +writer.account();

    builder << ", ";
    builder << R"("Symbol": )";
    builder << +writer.symbol();

    builder << ", ";
    builder << R"("Side": )";
    builder << '"' << writer.side() << '"';

    builder << ", ";
    builder << R"("TransactTime": )";
    builder << +writer.transactTime();

    builder << ", ";
    builder << R"("OrderQty": )";
    builder << writer.orderQty();

    builder << ", ";
    builder << R"("OrdType": )";
    builder << +writer.ordType();

    builder << ", ";
    builder << R"("Price": )";
    builder << writer.price();

    builder << '}';

    return builder;
  }

  void skip() {}

  SBE_NODISCARD static SBE_CONSTEXPR bool isConstLength() SBE_NOEXCEPT {
    return true;
  }

  SBE_NODISCARD static std::size_t computeLength() {
#if defined(__GNUG__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif
    std::size_t length = sbeBlockLength();

    return length;
#if defined(__GNUG__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
  }
};
}  // namespace sbe
#endif
