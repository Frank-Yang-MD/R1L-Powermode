/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import java.util.HashSet;
import java.util.Set;

/** ScriptCommandFactory creates IScriptCommand objects */
final class ScriptCommandFactory implements IScriptCommandFactory {
    private final Set<ICommandCreator> mCreators = new HashSet<>();

    /**
     * Add creator for commamd
     *
     * @param creator reference to creator
     */
    public void addCreator(ICommandCreator creator) {
        mCreators.add(creator);
    }

    @Override
    public IScriptCommand createCommand(String command, int lineNumber) {
        return mCreators.stream()
                .filter(cmd -> cmd.isMatch(command))
                .map(cmd -> cmd.create(command, lineNumber))
                .findFirst()
                .orElse(null);
    }
}
