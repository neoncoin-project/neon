// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NEON_QT_NEONADDRESSVALIDATOR_H
#define NEON_QT_NEONADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class NeonAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit NeonAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Neon address widget validator, checks for a valid neon address.
 */
class NeonAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit NeonAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // NEON_QT_NEONADDRESSVALIDATOR_H
