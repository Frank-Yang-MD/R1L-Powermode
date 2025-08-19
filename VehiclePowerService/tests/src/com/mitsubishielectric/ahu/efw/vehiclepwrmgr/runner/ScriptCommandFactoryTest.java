/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.*;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class ScriptCommandFactoryTest {
    private final String TEST_COMMAND = "test";
    private final ScriptCommandFactory mScriptCommandFactory = new ScriptCommandFactory();

    @Mock private ICommandCreator mCommandCreatorSpy;

    @Mock private IScriptCommand mScriptCommandMock;

    @Before
    public void before() {
        // Arrange
        when(mCommandCreatorSpy.isMatch(TEST_COMMAND)).thenReturn(true);
        when(mCommandCreatorSpy.create(TEST_COMMAND, 0)).thenReturn(mScriptCommandMock);
        mScriptCommandFactory.addCreator(mCommandCreatorSpy);
    }

    @Test
    public void createCommandWithEmptyFactory() {
        ScriptCommandFactory scriptCommandFactory = new ScriptCommandFactory();
        assertNull(scriptCommandFactory.createCommand("test", 0));
    }

    @Test
    public void createCommandWithInvalidString() {
        final String INVALID_COMMAND = "invalid command";

        // Act
        IScriptCommand command = mScriptCommandFactory.createCommand(INVALID_COMMAND, 0);

        // Assert
        assertNull(command);
        verify(mCommandCreatorSpy).isMatch(INVALID_COMMAND);
        verify(mCommandCreatorSpy, never()).create(anyString(), anyInt());
    }

    @Test
    public void createCommand() {
        // Act
        IScriptCommand command = mScriptCommandFactory.createCommand(TEST_COMMAND, 0);

        // Assert
        assertNotNull(command);
        verify(mCommandCreatorSpy).isMatch(TEST_COMMAND);
        verify(mCommandCreatorSpy).create(TEST_COMMAND, 0);
    }
}
