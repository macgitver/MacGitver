/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QObject>

namespace RM { namespace Backend {

class AbstractService;

enum class RunnerPriority
{
    Low,
    Default,
    Urgent,
    Immediate,
};

class ServiceRunner
        : public QObject
{
    Q_OBJECT
private:
    ServiceRunner();
public:
    ~ServiceRunner();

public:
    static ServiceRunner& instance();

public:
    void enqueue(Backend::AbstractService* svc, RunnerPriority priority);

signals:
    void processNextService();

private:
    void processNext();

private:
    class Data;
    Data* d;
};

static inline void perform(
        Backend::AbstractService* svc,
        RunnerPriority priority = RunnerPriority::Default)
{
    ServiceRunner::instance().enqueue(svc, priority);
}

} }
