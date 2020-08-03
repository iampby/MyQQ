#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>

static const unsigned char qt_resource_tree[] = {
0,
0,0,0,0,2,0,0,0,10,0,0,0,1,0,0,1,
146,0,2,0,0,0,18,0,0,0,17,0,0,4,24,0,
2,0,0,0,4,0,0,0,13,0,0,0,136,0,0,0,
0,0,1,0,0,0,0,0,0,0,76,0,0,0,0,0,
1,0,0,0,0,0,0,1,108,0,0,0,0,0,1,0,
0,0,0,0,0,0,48,0,0,0,0,0,1,0,0,0,
0,0,0,1,64,0,0,0,0,0,1,0,0,0,0,0,
0,0,8,0,0,0,0,0,1,0,0,0,0,0,0,0,
190,0,2,0,0,0,2,0,0,0,11,0,0,1,86,0,
0,0,0,0,1,0,0,0,0,0,0,0,212,0,0,0,
0,0,1,0,0,0,0,0,0,1,4,0,0,0,0,0,
1,0,0,0,0,0,0,4,114,0,0,0,0,0,1,0,
0,0,0,0,0,4,40,0,0,0,0,0,1,0,0,0,
0,0,0,4,82,0,0,0,0,0,1,0,0,0,0,0,
0,4,166,0,0,0,0,0,1,0,0,0,0,0,0,3,
174,0,0,0,0,0,1,0,0,0,0,0,0,2,158,0,
0,0,0,0,1,0,0,0,0,0,0,3,48,0,0,0,
0,0,1,0,0,0,0,0,0,2,38,0,0,0,0,0,
1,0,0,0,0,0,0,3,76,0,0,0,0,0,1,0,
0,0,0,0,0,2,70,0,0,0,0,0,1,0,0,0,
0,0,0,2,126,0,0,0,0,0,1,0,0,0,0,0,
0,3,240,0,0,0,0,0,1,0,0,0,0,0,0,1,
160,0,0,0,0,0,1,0,0,0,0,0,0,3,8,0,
0,0,0,0,1,0,0,0,0,0,0,2,4,0,0,0,
0,0,1,0,0,0,0,0,0,3,206,0,0,0,0,0,
1,0,0,0,0,0,0,2,180,0,0,0,0,0,1,0,
0,0,0,0,0,3,148,0,0,0,0,0,1,0,0,0,
0,0,0,2,98,0,0,0,0,0,1,0,0,0,0,0,
0,3,118,0,0,0,0,0,1,0,0,0,0,0,0,2,
222,0,0,0,0,0,1,0,0,0,0,0,0,1,202,0,
0,0,0,0,1,0,0,0,0};
static const unsigned char qt_resource_names[] = {
0,
1,0,0,0,47,0,47,0,17,10,157,25,156,0,77,0,
111,0,117,0,115,0,101,0,70,0,111,0,114,0,83,0,
104,0,97,0,112,0,101,0,46,0,113,0,109,0,108,0,
11,6,221,2,60,0,65,0,99,0,116,0,105,0,111,0,
110,0,115,0,46,0,113,0,109,0,108,0,27,5,14,82,
156,0,77,0,111,0,117,0,115,0,101,0,77,0,111,0,
118,0,101,0,67,0,117,0,115,0,116,0,111,0,109,0,
70,0,111,0,114,0,73,0,110,0,110,0,101,0,114,0,
46,0,113,0,109,0,108,0,24,0,189,76,92,0,77,0,
111,0,117,0,115,0,101,0,67,0,117,0,115,0,116,0,
111,0,109,0,70,0,111,0,114,0,87,0,105,0,110,0,
100,0,111,0,119,0,46,0,113,0,109,0,108,0,8,11,
224,165,194,0,114,0,101,0,103,0,105,0,115,0,116,0,
101,0,114,0,21,15,13,181,252,0,82,0,101,0,103,0,
105,0,115,0,116,0,101,0,114,0,73,0,110,0,116,0,
101,0,114,0,102,0,97,0,99,0,101,0,46,0,113,0,
109,0,108,0,27,15,148,125,220,0,82,0,101,0,103,0,
105,0,115,0,116,0,101,0,114,0,73,0,110,0,102,0,
76,0,97,0,110,0,103,0,77,0,105,0,110,0,67,0,
108,0,111,0,115,0,101,0,46,0,113,0,109,0,108,0,
8,8,1,90,92,0,109,0,97,0,105,0,110,0,46,0,
113,0,109,0,108,0,8,12,70,97,156,0,70,0,117,0,
110,0,99,0,46,0,113,0,109,0,108,0,16,5,192,169,
115,0,99,0,104,0,105,0,110,0,101,0,115,0,101,0,
45,0,108,0,117,0,110,0,97,0,114,0,46,0,106,0,
115,0,4,0,7,55,254,0,109,0,97,0,105,0,110,0,
18,7,120,108,124,0,87,0,101,0,97,0,116,0,104,0,
101,0,114,0,67,0,105,0,116,0,121,0,82,0,101,0,
99,0,46,0,113,0,109,0,108,0,26,14,207,101,220,0,
77,0,101,0,115,0,115,0,97,0,103,0,101,0,68,0,
105,0,97,0,108,0,111,0,103,0,70,0,111,0,114,0,
83,0,119,0,105,0,116,0,99,0,104,0,46,0,113,0,
109,0,108,0,14,11,20,155,188,0,87,0,101,0,97,0,
116,0,104,0,101,0,114,0,87,0,105,0,110,0,46,0,
113,0,109,0,108,0,13,1,81,199,188,0,65,0,98,0,
111,0,117,0,116,0,77,0,121,0,81,0,81,0,46,0,
113,0,109,0,108,0,11,4,73,36,60,0,69,0,100,0,
117,0,73,0,110,0,102,0,111,0,46,0,113,0,109,0,
108,0,11,13,156,210,156,0,72,0,101,0,108,0,112,0,
87,0,105,0,110,0,46,0,113,0,109,0,108,0,13,4,
144,174,156,0,86,0,101,0,114,0,105,0,102,0,121,0,
87,0,105,0,110,0,46,0,113,0,109,0,108,0,8,0,
167,94,92,0,76,0,105,0,115,0,116,0,46,0,113,0,
109,0,108,0,18,11,170,34,124,0,73,0,110,0,102,0,
111,0,83,0,109,0,97,0,108,0,108,0,76,0,97,0,
98,0,101,0,108,0,46,0,113,0,109,0,108,0,18,14,
23,222,28,0,84,0,101,0,109,0,112,0,101,0,114,0,
97,0,116,0,117,0,114,0,101,0,66,0,116,0,110,0,
46,0,113,0,109,0,108,0,17,9,45,184,156,0,66,0,
117,0,116,0,116,0,111,0,110,0,70,0,111,0,114,0,
77,0,101,0,110,0,117,0,46,0,113,0,109,0,108,0,
11,0,212,59,28,0,72,0,101,0,97,0,100,0,73,0,
110,0,102,0,46,0,113,0,109,0,108,0,18,3,200,22,
124,0,73,0,110,0,100,0,105,0,118,0,105,0,100,0,
117,0,97,0,108,0,68,0,97,0,116,0,97,0,46,0,
113,0,109,0,108,0,12,13,168,70,60,0,69,0,100,0,
105,0,116,0,73,0,110,0,102,0,111,0,46,0,113,0,
109,0,108,0,10,12,78,233,220,0,70,0,114,0,105,0,
101,0,110,0,100,0,46,0,113,0,109,0,108,0,13,0,
79,220,252,0,65,0,108,0,116,0,101,0,114,0,72,0,
73,0,109,0,103,0,46,0,113,0,109,0,108,0,14,11,
157,146,252,0,65,0,108,0,116,0,101,0,114,0,67,0,
111,0,118,0,101,0,114,0,46,0,113,0,109,0,108,0,
17,6,239,201,124,0,109,0,97,0,105,0,110,0,73,0,
110,0,116,0,101,0,114,0,102,0,97,0,99,0,101,0,
46,0,113,0,109,0,108,0,5,0,115,93,254,0,108,0,
111,0,103,0,105,0,110,0,18,11,246,73,60,0,76,0,
111,0,103,0,105,0,110,0,73,0,110,0,116,0,101,0,
114,0,102,0,97,0,99,0,101,0,46,0,113,0,109,0,
108,0,13,13,187,84,28,0,76,0,111,0,103,0,105,0,
110,0,69,0,100,0,105,0,116,0,46,0,113,0,109,0,
108,0,23,3,101,185,156,0,76,0,111,0,103,0,105,0,
110,0,84,0,111,0,116,0,97,0,108,0,73,0,110,0,
116,0,101,0,114,0,102,0,97,0,99,0,101,0,46,0,
113,0,109,0,108,0,21,15,49,49,124,0,76,0,111,0,
103,0,105,0,110,0,83,0,101,0,116,0,73,0,110,0,
116,0,101,0,114,0,102,0,97,0,99,0,101,0,46,0,
113,0,109,0,108};
static const unsigned char qt_resource_empty_payout[] = { 0, 0, 0, 0, 0 };
QT_BEGIN_NAMESPACE
extern Q_CORE_EXPORT bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
QT_END_NAMESPACE
namespace QmlCacheGeneratedCode {
namespace _login_LoginSetInterface_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_mainInterface_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _login_LoginTotalInterface_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_AlterCover_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_AlterHImg_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_Friend_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_EditInfo_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__chinese_0x2d_lunar_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_IndividualData_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_HeadInf_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__Func_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_ButtonForMenu_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_TemperatureBtn_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_InfoSmallLabel_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__main_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _register_RegisterInfLangMinClose_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__MouseCustomForWindow_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_List_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_VerifyWin_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_HelpWin_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__MouseMoveCustomForInner_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__Actions_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_EduInfo_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _login_LoginEdit_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _register_RegisterInterface_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_AboutMyQQ_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _login_LoginInterface_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_WeatherWin_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_MessageDialogForSwitch_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _main_WeatherCityRec_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__MouseForShape_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
        resourcePathToCachedUnit.insert(QStringLiteral("/login/LoginSetInterface.qml"), &QmlCacheGeneratedCode::_login_LoginSetInterface_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/mainInterface.qml"), &QmlCacheGeneratedCode::_main_mainInterface_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/login/LoginTotalInterface.qml"), &QmlCacheGeneratedCode::_login_LoginTotalInterface_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/AlterCover.qml"), &QmlCacheGeneratedCode::_main_AlterCover_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/AlterHImg.qml"), &QmlCacheGeneratedCode::_main_AlterHImg_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/Friend.qml"), &QmlCacheGeneratedCode::_main_Friend_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/EditInfo.qml"), &QmlCacheGeneratedCode::_main_EditInfo_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/chinese-lunar.js"), &QmlCacheGeneratedCode::_0x5f__chinese_0x2d_lunar_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/IndividualData.qml"), &QmlCacheGeneratedCode::_main_IndividualData_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/HeadInf.qml"), &QmlCacheGeneratedCode::_main_HeadInf_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/Func.qml"), &QmlCacheGeneratedCode::_0x5f__Func_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/ButtonForMenu.qml"), &QmlCacheGeneratedCode::_main_ButtonForMenu_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/TemperatureBtn.qml"), &QmlCacheGeneratedCode::_main_TemperatureBtn_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/InfoSmallLabel.qml"), &QmlCacheGeneratedCode::_main_InfoSmallLabel_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main.qml"), &QmlCacheGeneratedCode::_0x5f__main_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/register/RegisterInfLangMinClose.qml"), &QmlCacheGeneratedCode::_register_RegisterInfLangMinClose_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/MouseCustomForWindow.qml"), &QmlCacheGeneratedCode::_0x5f__MouseCustomForWindow_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/List.qml"), &QmlCacheGeneratedCode::_main_List_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/VerifyWin.qml"), &QmlCacheGeneratedCode::_main_VerifyWin_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/HelpWin.qml"), &QmlCacheGeneratedCode::_main_HelpWin_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/MouseMoveCustomForInner.qml"), &QmlCacheGeneratedCode::_0x5f__MouseMoveCustomForInner_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/Actions.qml"), &QmlCacheGeneratedCode::_0x5f__Actions_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/EduInfo.qml"), &QmlCacheGeneratedCode::_main_EduInfo_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/login/LoginEdit.qml"), &QmlCacheGeneratedCode::_login_LoginEdit_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/register/RegisterInterface.qml"), &QmlCacheGeneratedCode::_register_RegisterInterface_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/AboutMyQQ.qml"), &QmlCacheGeneratedCode::_main_AboutMyQQ_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/login/LoginInterface.qml"), &QmlCacheGeneratedCode::_login_LoginInterface_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/WeatherWin.qml"), &QmlCacheGeneratedCode::_main_WeatherWin_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/MessageDialogForSwitch.qml"), &QmlCacheGeneratedCode::_main_MessageDialogForSwitch_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/main/WeatherCityRec.qml"), &QmlCacheGeneratedCode::_main_WeatherCityRec_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/MouseForShape.qml"), &QmlCacheGeneratedCode::_0x5f__MouseForShape_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.version = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
QT_PREPEND_NAMESPACE(qRegisterResourceData)(/*version*/0x01, qt_resource_tree, qt_resource_names, qt_resource_empty_payout);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qml)() {
    ::unitRegistry();
    Q_INIT_RESOURCE(qml_qmlcache);
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qml))
int QT_MANGLE_NAMESPACE(qCleanupResources_qml)() {
    Q_CLEANUP_RESOURCE(qml_qmlcache);
    return 1;
}
