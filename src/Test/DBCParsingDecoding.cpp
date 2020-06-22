#include <fstream>
#include <iostream>
#include <functional>
#include <ctime>
#include <chrono>
#include <string>

#include "Config.h"

#include "../../include/dbcppp/Network.h"
#include <boost/test/unit_test.hpp>
#include <boost/endian/conversion.hpp>

namespace utf = boost::unit_test;

double _easy_decode(dbcppp::Signal &sig, std::vector<uint8_t> &data) {
    if (sig.getBitSize() == 0) {
        return 0;
    }
    union {
        uint64_t ui;
        int64_t i;
        float f;
        double d;
    } retVal{0};

    if (sig.getByteOrder() == dbcppp::Signal::ByteOrder::BigEndian) {
        auto srcBit = sig.getStartBit();
        auto dstBit = sig.getBitSize() - 1;
        for (auto i = 0; i < sig.getBitSize(); ++i) {
            if (data[srcBit / 8] & (1ull << (srcBit % 8))) {
                retVal.ui |= (1ULL << dstBit);
            }
            if ((srcBit % 8) == 0) {
                srcBit += 15;
            } else {
                --srcBit;
            }
            --dstBit;
        }
    }

    else
    {
        auto srcBit = sig.getStartBit();
        auto dstBit = 0;
        for (auto i = 0; i < sig.getBitSize(); ++i) {
            if (data[srcBit / 8] & (1ULL << (srcBit % 8))) {
                retVal.ui |= (1ULL << dstBit);
            }
            ++srcBit;
            ++dstBit;
        }
    }

    switch (sig.getExtendedValueType()) {
        case dbcppp::Signal::ExtendedValueType::Float:
            return retVal.f;
        case dbcppp::Signal::ExtendedValueType::Double:
            return retVal.d;
    }
    if (sig.getValueType() == dbcppp::Signal::ValueType::Signed) {
        if (retVal.ui & (1ull << (sig.getBitSize() - 1))) {
            for (auto i = sig.getBitSize(); i < 64; ++i) {
                retVal.ui |= (1ULL << i);
            }
        }
        return double(retVal.i);
    }
    return double(retVal.ui);
}

//BOOST_AUTO_TEST_CASE(Test_decoding) {
//    using namespace dbcppp;
//
//    std::vector<uint8_t> data
//    {
//            0, 0, 0, 0, 99, 155, 245, 149,
//            8, 118, 32, 0, 0, 0, 0, 128,
//            0, 0, 0, 0, 227, 174, 36, 62,
//            41, 100, 1, 0, 50, 0, 16, 14,
//            0, 0, 0, 0, 98, 144, 1, 1,
//            144, 229, 152, 37, 2, 185, 13, 9,
//            123, 211, 1, 1, 195, 43, 192, 101,
//            1, 58, 11, 16, 203, 18, 12, 25,
//            23, 128, 172, 64, 86, 105, 9, 4,
//            209, 43, 64, 0, 136, 85, 79, 16,
//            136, 120, 0, 1, 57, 214, 1, 1,
//            71, 21, 80, 2, 173, 40, 83, 24,
//            66, 32, 51, 24, 67, 32, 35, 24,
//            0, 96, 134, 16, 66, 224, 184, 15,
//            70, 96, 1, 0
//    };
//
////    for (int x : data)
////        std::cout << x << " ";
////    std::cout << std::endl;
//
//    {
//        BOOST_TEST_MESSAGE("OBJ_Zero_byte");
//        std::unique_ptr<Signal> OBJ_Zero_byte = Signal::create(1940, "OBJ_Zero_byte", Signal::Multiplexer::NoMux, 0, 0,
//                                                               32,
//                                                               Signal::ByteOrder::LittleEndian,
//                                                               Signal::ValueType::Unsigned, 1.0, 0.0, 0.0, 255.0,
//                                                               "", {}, {}, {}, "",
//                                                               Signal::ExtendedValueType::Integer);
//
//        double dec_easy_decoded_obj_zero_byte = easy_decode(*OBJ_Zero_byte, data);
//        double dec_sig_decoded_obj_zero_byte = OBJ_Zero_byte->decode(&data[0]);
//        BOOST_TEST(dec_easy_decoded_obj_zero_byte == dec_sig_decoded_obj_zero_byte);
//        BOOST_TEST(dec_easy_decoded_obj_zero_byte == 0);
//    }
//
//    {
//        BOOST_TEST_MESSAGE("OBJ_Triggered_SDM_0");
//        std::unique_ptr<Signal> OBJ_Triggered_SDM_0 = Signal::create(1940, "OBJ_Triggered_SDM_0",
//                                                                     Signal::Multiplexer::NoMux, 0, 224, 2,
//                                                                     Signal::ByteOrder::LittleEndian,
//                                                                     Signal::ValueType::Unsigned, 1.0, 0.0, 0.0, 2.0,
//                                                                     "Enum", {}, {}, {}, "",
//                                                                     Signal::ExtendedValueType::Integer);
//
//        double dec_easy_object_triggered_sdm = easy_decode(*OBJ_Triggered_SDM_0, data);
//        double dec_sig_object_triggered_sdm = OBJ_Triggered_SDM_0->decode(&data[0]);
//
//        BOOST_TEST(dec_easy_object_triggered_sdm == dec_sig_object_triggered_sdm);
//        BOOST_TEST(OBJ_Triggered_SDM_0->rawToPhys(dec_sig_object_triggered_sdm) == 2);
//    }
//
//    {
//        BOOST_TEST_MESSAGE("OBJ_Protocol_Version");
//        std::unique_ptr<Signal> OBJ_Protocol_Version = Signal::create(1940, "OBJ_Protocol_Version",
//                Signal::Multiplexer::NoMux, 0, 64,8,
//                                                                      Signal::ByteOrder::LittleEndian,  Signal::ValueType::Unsigned, 1.0, 0.0, 8.0, 8.0, "counter", {}, {}, {}, "",
//                                                                      Signal::ExtendedValueType::Integer);
//
//        double dec_easy_object_protocol_version = easy_decode(*OBJ_Protocol_Version, data);
//        double dec_object_protocol_version = OBJ_Protocol_Version->decode(&data[0]);
//
//        BOOST_TEST(dec_easy_object_protocol_version == dec_object_protocol_version);
//        BOOST_TEST(OBJ_Protocol_Version->rawToPhys(dec_object_protocol_version) == 8);
//    }
//    {
//        BOOST_TEST_MESSAGE("OBJ_Height_0");
//        std::unique_ptr<Signal> OBJ_Height_0 = Signal::create(1940, "OBJ_Height_0",
//                                                                      Signal::Multiplexer::NoMux, 0, 361,9,
//                                                                      Signal::ByteOrder::LittleEndian,  Signal::ValueType::Unsigned, 0.01, 0.0, 8.0, 8.0, "counter", {}, {}, {}, "",
//                                                                      Signal::ExtendedValueType::Integer);
//
//        double dec_easy_object_height_0 = easy_decode(*OBJ_Height_0, data);
//        double dec_object_height_0 = OBJ_Height_0->decode(&data[0]);
//
//        BOOST_TEST(dec_easy_object_height_0 == dec_object_height_0);
//        BOOST_TEST(OBJ_Height_0->rawToPhys(dec_object_height_0) == 2.2);
//    }
//}

//BOOST_AUTO_TEST_CASE(DBCParsing) {
//
//    BOOST_TEST_MESSAGE("Testing DBC AST tree for correctness...");
//
//    std::ifstream lane_host(Core_Lanes_Host_protocol);
//    std::ifstream object(Core_Objects_protocol);
//
//    auto net = dbcppp::Network::fromDBC(lane_host);
//    net = dbcppp::Network::fromDBC(object, std::move(net));
//
//    net->forEachMessage
//    (
//        [&](const dbcppp::Message &m)
//        {
//            std::cout << m.getName() << std::endl;
//            std::cout << "for Each Signal by Start Bit" << std::endl;
//            m.forEachSignalOrderedByStartBit
//            (
//                [&](const dbcppp::Signal &s)
//                {
//                    s.forEachValueDescription
//                    (
//                    [&](double value, const std::string& desc)
//                        {
//                            std::cout << " value: " << value << ", desc: " << desc << std::endl;
//                        });
//                    std::cout << " Name: " << s.getName() << ", Start bit: " << s.getStartBit()
//                              << ", Max: "
//                              << s.getMaximum() << ", Bitsize: " << s.getBitSize()
//                              << ", Comment: "
//                              << s.getComment()
//                              << std::endl;
//                }
//            );
//
//            std::cout << "for Each Signal" << std::endl;
//            m.forEachSignal
//            (
//                    [&](const dbcppp::Signal &s)
//                    {
//                        std::cout << " Name: " << s.getName() << ", Start bit: " << s.getStartBit()
//                                  << ", Max: "
//                                  << s.getMaximum() << ", Bitsize: " << s.getBitSize()
//                                  << ", Comment: "
//                                  << s.getComment() << std::endl;
//                    }
//            );
//        }
//    );
//
//    BOOST_TEST_MESSAGE("Done!");
//}

BOOST_AUTO_TEST_CASE(DBCParsingAndDecoding)
{
    using namespace dbcppp;
    BOOST_TEST_MESSAGE("DBCParsingAndDecoding");
    std::vector<uint8_t> data
    {
            0, 0, 0, 0, 99, 155, 245, 149,
            8, 118, 32, 0, 0, 0, 0, 128,
            0, 0, 0, 0, 227, 174, 36, 62,
            41, 100, 1, 0, 50, 0, 16, 14,
            0, 0, 0, 0, 98, 144, 1, 1,
            144, 229, 152, 37, 2, 185, 13, 9,
            123, 211, 1, 1, 195, 43, 192, 101,
            1, 58, 11, 16, 203, 18, 12, 25,
            23, 128, 172, 64, 86, 105, 9, 4,
            209, 43, 64, 0, 136, 85, 79, 16,
            136, 120, 0, 1, 57, 214, 1, 1,
            71, 21, 80, 2, 173, 40, 83, 24,
            66, 32, 51, 24, 67, 32, 35, 24,
            0, 96, 134, 16, 66, 224, 184, 15,
            70, 96, 1, 0
    };

    std::ifstream object(Core_Objects_protocol);
    auto net = dbcppp::Network::fromDBC(object);

    //    std::unique_ptr<Signal> OBJ_Triggered_SDM_0 = Signal::create(1940, "OBJ_Triggered_SDM_0",
//                                                                 Signal::Multiplexer::NoMux, 0, 224, 2,
//                                                                 Signal::ByteOrder::LittleEndian,
//                                                                 Signal::ValueType::Unsigned, 1.0, 0.0, 0.0, 2.0,
//                                                                 "Enum", {}, {}, {}, "",
//                                                                 Signal::ExtendedValueType::Integer);

    BOOST_TEST_MESSAGE("OBJ_Triggered_SDM_0");
    auto* message = net->getMessageById(154);
    auto* OBJ_Triggered_SDM_0 = message->getSignalByName("OBJ_Triggered_SDM_0");

    double dec_easy_object_triggered_sdm = _easy_decode(*const_cast<Signal*>(OBJ_Triggered_SDM_0), data);
    double dec_sig_object_triggered_sdm = OBJ_Triggered_SDM_0->decode(&data[0]);

    BOOST_TEST(dec_easy_object_triggered_sdm == dec_sig_object_triggered_sdm);
    BOOST_TEST(OBJ_Triggered_SDM_0->rawToPhys(dec_sig_object_triggered_sdm) == 2);
    auto&& y = OBJ_Triggered_SDM_0->getValueDescriptionByValue(2);
    if(y != nullptr)
    {
        std::cout << *y << std::endl;
    }
    else
    {
        std::cout << "nil" << std::endl;
    }

    BOOST_TEST_MESSAGE("OBJ_Height_0");
    auto* OBJ_Height_0 = message->getSignalByName("OBJ_Height_0");

    double dec_easy_object_height_0 = _easy_decode(*const_cast<Signal*>(OBJ_Height_0), data);
    double dec_sig_object_height_0 = OBJ_Height_0->decode(&data[0]);

    BOOST_TEST(dec_easy_object_height_0 == dec_sig_object_height_0);
    BOOST_TEST(OBJ_Height_0->rawToPhys(dec_sig_object_height_0) == 2.2);
    auto&& x =  OBJ_Height_0->getValueDescriptionByValue(1);
    if(x != nullptr)
    {
        std::cout << *x << std::endl;
    }
    else
    {
        std::cout << "nil" << std::endl;
    }

}
