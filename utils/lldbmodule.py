#!/usr/bin/env python

import lldb
import optparse
import shlex
from dataclasses import dataclass

@dataclass
class ProcessInfo:
    name: str
    address: int

def loadProgramSymbols(debugger: lldb.SBDebugger, command, result, internal_dict):
    process = debugger.GetSelectedTarget().GetProcess()
    def getProcessFromPID(pid: int):
        frame = process.GetSelectedThread().GetFrameAtIndex(0)
        if frame.EvaluateExpression(f'processes[{pid}].state').GetValue() == 'PROCESS_DEAD':
            return None
        def parseAddress(add: str):
            return int(add[2:], 16)
        name = parseAddress(frame.EvaluateExpression(f'processes[{pid}].name').GetValue())
        name = process.ReadCStringFromMemory(name, 32, lldb.SBError())
        entryPoint = parseAddress(frame.EvaluateExpression(f'processes[{pid}].entryPoint').GetValue())

        return ProcessInfo(name, entryPoint)
    # process.ReadMemory(0x900000, )
    target: lldb.SBTarget = debugger.GetSelectedTarget()
    while target.GetNumModules() > 1:
        target.RemoveModule(target.GetModuleAtIndex(1))

    for pid in range(1, 128):
        info = getProcessFromPID(pid)
        if info is not None:
            print(f'{pid}: {info.name} at 0x{info.address:x}', file=result, flush=True)
            baseName = f'{info.name}.bin.elf'
            elfName = f'Userland/BUILD/{baseName}'
            uuid = f'{info.name} ({pid})'
            module = target.AddModule(elfName, 'x86_64-*-*', uuid)
            # print(module.sections[0].name)
            # print(module.FindSection('PT_LOAD[0]'))
            target.SetSectionLoadAddress(module.FindSection('PT_LOAD[0]'), info.address)
            # debugger.HandleCommand(f'target modules add "{elfName}"')
            # debugger.HandleCommand(f'target modules load --file {baseName} --uuid {uuid} --slide 0x{info.address:x}')

# def queue(debugger: lldb.SBDebugger, command, result, internal_dict):
    # process = debugger.GetSelectedTarget().GetProcess()
    # def getProcessFromPID(pid: int):
        # frame = process.GetSelectedThread().GetFrameAtIndex(0)
    

# And the initialization code to add your commands
def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f lldbmodule.loadProgramSymbols loadProgramSymbols')
    print('Loaded module.')
