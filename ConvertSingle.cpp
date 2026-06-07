#include <cstdint>
#include <iostream>
#include <vector>

#include "Utils/Config.h"
#include "Utils/Util.h"
#include "IccProfLib/IccCmm.h"
#include "IccProfLib/IccProfile.h"
#include "IccProfLib/icProfileHeader.h"

int main(int argc, char *argv[]) {
    std::vector<std::string> commandLineArgs{};

    // skip the program name itself by indexing from 1
    for (std::uint32_t i{1}; i < argc; ++i) {
        commandLineArgs.emplace_back(argv[i]);
    }

    Utils::Util util{};
    Utils::Config const config{util.parseCommandLine(commandLineArgs)};

    CIccProfile *profile = ReadIccProfile(config.getProfile().c_str());

    if (!profile) {
        return icCmmStatCantOpenProfile;
    }

    if (profile->m_Header.deviceClass != icSigInputClass &&
        profile->m_Header.deviceClass != icSigDisplayClass &&
        profile->m_Header.deviceClass != icSigOutputClass &&
        profile->m_Header.deviceClass != icSigColorSpaceClass) {
        return icCmmStatInvalidProfile;
    }

    const auto intent = static_cast<icRenderingIntent>(static_cast<std::uint32_t>(config.getRenderIntent()));
    const auto sourceSpace = config.isDeviceToPcs() ? profile->m_Header.colorSpace : profile->m_Header.pcs;
    const auto destinationSpace = config.isDeviceToPcs() ? profile->m_Header.pcs : profile->m_Header.colorSpace;
    CIccCmm cmm(sourceSpace, destinationSpace, config.isDeviceToPcs());

    // unclear why this is a choice, and set to false in ICC roundtrip demo ("useMPE" - multiProcessElement)
    // spec says DToB takes precedence over all other tags...
    // ... "except where this tag is not needed or supported by the CMM"
    // does that mean it's valid to choose not to implement DToB conversion and simply not support it? 🤷
    constexpr auto useDToBTags = false;

    // additionally, who determines this? LutColor might be more appropriate
    // ("a combination of icXformLutColorimetric with icXformLutSpectral")
    // since LutColorimetric appears to disable use of DToB tags in some cases for some reason? 🤷
    constexpr auto icXformLutType = icXformLutColorimetric;

    int outputChannels;
    if (config.isDeviceToPcs()) {
        outputChannels = 3;
    } else {
        switch (profile->m_Header.colorSpace) {
            case icSigGrayData:
            case icSig1colorData:
                outputChannels = 1;
                break;
            case icSig2colorData:
                outputChannels = 2;
                break;
            case icSigRgbData:
            case icSigXYZData:
            case icSigLabData:
            case icSigLuvData:
            case icSigYxyData:
            case icSigHsvData:
            case icSigHlsData:
            case icSigYCbCrData:
            case icSigCmyData:
            case icSig3colorData:
                outputChannels = 3;
                break;
            case icSigCmykData:
            case icSig4colorData:
                outputChannels = 4;
                break;
            case icSig5colorData:
                outputChannels = 5;
                break;
            case icSig6colorData:
                outputChannels = 6;
                break;
            case icSig7colorData:
                outputChannels = 7;
                break;
            case icSig8colorData:
                outputChannels = 8;
                break;
            case icSig9colorData:
                outputChannels = 9;
                break;
            case icSig10colorData:
                outputChannels = 10;
                break;
            case icSig11colorData:
                outputChannels = 11;
                break;
            case icSig12colorData:
                outputChannels = 12;
                break;
            case icSig13colorData:
                outputChannels = 13;
                break;
            case icSig14colorData:
                outputChannels = 14;
                break;
            case icSig15colorData:
                outputChannels = 15;
                break;
            default:
                throw std::invalid_argument(std::to_string(profile->m_Header.colorSpace) + " is not a supported colour space enum");
        }
    }

    // TODO: the actual CMM srcPixel -> dstPixel should probably be a function shared between ConvertBulk and ConvertSingle
    auto result = cmm.AddXform(profile, intent, icInterpLinear, nullptr, icXformLutType, useDToBTags);
    if (result != icCmmStatOk) {
        return result;
    }

    result = cmm.Begin();
    if (result != icCmmStatOk) {
        return result;
    }

    icFloatNumber srcPixel[15];
    icFloatNumber dstPixel[15];

    auto inputData = config.getInputSingleData();
    for (std::size_t i = 0; i < 15; ++i) {
        srcPixel[i] = static_cast<icFloatNumber>(inputData[i]);
    }

    cmm.Apply(dstPixel, srcPixel);

    if (profile->m_Header.pcs == icSigLabData) {
        icLabFromPcs(dstPixel);
    }
    else if (profile->m_Header.pcs == icSigXYZData) {
        icXyzFromPcs(dstPixel);
    }

    for (auto i = 0; i < outputChannels; ++i) {
        std::cout << std::fixed << dstPixel[i] << " ";
    }
    std::cout << std::endl;

    return icCmmStatOk;
}
