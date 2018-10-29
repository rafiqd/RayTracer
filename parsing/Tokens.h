//
// Created by Rafiq on 2018-10-27.
//

#ifndef RENDERER_TOKENS_H
#define RENDERER_TOKENS_H

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <bits/unique_ptr.h>

class Token {
protected:
    const std::string value;
    const std::string tokentype;
public:
    explicit Token(const std::string v, const std::string t): value(v), tokentype(t) {}
    virtual ~Token() = default;
    virtual const std::string& getValue() const = 0;
    const inline std::string& type() const { return tokentype; }
    friend inline std::ostream& operator<<(std::ostream& out, const Token &t){
        return out << t.tokentype << "[ " << t.getValue() << " ]";
    }
};

class ReserveWordToken : public Token {
public:
    explicit ReserveWordToken(const std::string &v): Token(v, "reserve") {}
    const inline std::string& getValue() const override { return value; }

};

class ValueToken : public Token {
public:
    explicit ValueToken(const std::string &v): Token(v, "value") {}
    const inline std::string& getValue() const override { return value; }
};

class FieldToken : public Token {
public:
    explicit FieldToken(const std::string &v): Token(v, "field") {}
    const inline std::string& getValue() const override { return value; }
};

class TokenList {
    unsigned counter;
    std::vector<std::unique_ptr<Token>> tokens;
public:
    TokenList(): counter(0) {}
    TokenList(const TokenList&) = delete;
    TokenList&operator=(const TokenList&) = delete;

    void inline addValueToken(const std::string& t){
        tokens.push_back(std::make_unique<ValueToken>(t));
    }

    void inline addReserveToken(const std::string& t){
        tokens.push_back(std::make_unique<ReserveWordToken>(t));
    }

    void inline addFieldToken(const std::string& t){
        tokens.push_back(std::make_unique<FieldToken>(t));
    }

    inline const Token* peakNextToken(){
        return counter >= tokens.size() ? nullptr : tokens[counter].get();
    }

    inline const Token* getNextToken() {
        if (counter >= tokens.size()){
            throw 1;
        }
        auto t = tokens[counter].get();
        counter++;
        return t;
    }

    const Token* getPrevToken() {
        assert(counter > 0);
        --counter;
        return tokens[counter].get();
    }

    friend inline std::ostream& operator<<(std::ostream& out, const TokenList &tl) {
        out << "TokenList[ ";
        for(auto &t: tl.tokens){
            out << *t << ", ";
        }
        out << "]";
        return out;
    }

};


void tokenize(std::string, TokenList&);
#endif //RENDERER_TOKENS_H
