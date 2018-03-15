#ifndef MK_PLAYER_DATA_H
#define MK_PLAYER_DATA_H

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/GameData/MKGameData.h"
#include "MKBackgroundData.h"

// Include STL
#include <cmath>
#include <vector>
#include <set>

// Namespaces
using namespace std;

NS_MK_BEGIN

class MKPlayerData : public MKGameData
{
private:
    // Highscore & Coins
    mkU64 m_Highscore = 0;
    mkU64 m_Coins = 0;

    mkBool LoadHighscore(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool SaveHighscore(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool LoadCoins(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool SaveCoins(RAPIDJSON_NAMESPACE::Document& _dataDocument);

    // Owned/Equipped Background
    mkString m_EquippedBackground = MKBackgroundData::DEFAULT_BACKGROUND_NAME;
    std::set<mkString> m_OwnedBackgrounds;

    mkBool LoadEquippedBackground(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool SaveEquippedBackground(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool LoadOwnedBackgrounds(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool SaveOwnedBackgrounds(RAPIDJSON_NAMESPACE::Document& _dataDocument);

    // Owned/Equipped Hero
    mkString m_EquippedHero = "Default";
    std::set<mkString> m_OwnedHeroes;

    mkBool LoadEquippedHero(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool SaveEquippedHero(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool LoadOwnedHeroes(RAPIDJSON_NAMESPACE::Document& _dataDocument);
    mkBool SaveOwnedHeroes(RAPIDJSON_NAMESPACE::Document& _dataDocument);

public:
    // Contructor(s) & Destructor
    MKPlayerData() {}
    virtual ~MKPlayerData() {}

    // JSON Data Name(s)
    static const mkString HIGHSCORE_JSON_DATA_NAME;
    static const mkString COINS_JSON_DATA_NAME;

    // Backgrounds
    static const mkString EQUIPPED_BACKGROUND_JSON_DATA_NAME;
    static const mkString OWNED_BACKGROUNDS_JSON_DATA_NAME;

    // Heroes
    static const mkString EQUIPPED_HERO_JSON_DATA_NAME;
    static const mkString OWNED_HEROES_JSON_DATA_NAME;

    virtual mkBool LoadData(const mkString& _filePath) override;
    virtual mkBool SaveData(const mkString& _filePath) override;

    virtual mkString GetWritablePath() const override;
    virtual mkString GetCachedPath() const override;

    inline mkU64 GetHighscore() const { return m_Highscore; }
    inline void SetHighscore(mkU64 _score) { m_Highscore = _score; }

    inline mkU64 GetCoins() const { return m_Coins; }
    inline void SetCoins(mkU64 _coins) { m_Coins = _coins; }
    inline void AddCoins(mkU64 _coins) { m_Coins += _coins; }
    inline void RemoveCoins(mkU64 _coins) { m_Coins -= _coins; }
    
    // Backgrounds
    inline const mkString& GetEquippedBackground() const { return m_EquippedBackground; }
    inline void SetEquippedBackground(const mkString& _backgroundName) { m_EquippedBackground = _backgroundName; }

    inline const std::set<mkString>& GetOwnedBackgrounds() const { return m_OwnedBackgrounds; }
    inline void SetOwnedBackgrounds(const set<mkString>& _backgroundNames) { m_OwnedBackgrounds = _backgroundNames; }
    inline void AddOwnedBackground(const mkString& _backgroundName) { m_OwnedBackgrounds.insert(_backgroundName); }
    inline mkBool OwnsBackground(const mkString& _backgroundName) const { return m_OwnedBackgrounds.find(_backgroundName) != m_OwnedBackgrounds.end(); }

    // Heroes
    inline const mkString& GetEquippedHero() const { return m_EquippedHero; }
    inline void SetEquippedHero(const mkString& _heroName) { m_EquippedHero = _heroName; }

    inline const std::set<mkString>& GetOwnedHeroes() const { return m_OwnedHeroes; }
    inline void SetOwnedHeroes(const set<mkString>& _heroNames) { m_OwnedHeroes = _heroNames; }
    inline void AddOwnedHero(const mkString& _heroName) { m_OwnedHeroes.insert(_heroName); }
    inline mkBool OwnsHero(const mkString& _heroName) const { return m_OwnedHeroes.find(_heroName) != m_OwnedHeroes.end(); }
};

NS_MK_END

#endif // MK_PLAYER_DATA_H