#!/bin/sh

./sendtocli http://127.0.0.1:1234/auto/duchangecomplete/ "data=<Results><UUID>1</UUID><DeploymentUnitRef>duref</DeploymentUnitRef><Version>1.0</Version><CurrentState>Installed</CurrentState><Resolved>1</Resolved><ExecutionUnitRefList>a.b.eu,c.d.eu</ExecutionUnitRefList><StartTime>2009-09-25T00:14:52.421599</StartTime><CompleteTime>2009-09-25T00:14:52.421599</CompleteTime><Fault><FaultCode>0</FaultCode><FaultString></FaultString></Fault><OperationPerformed>Install</OperationPerformed></Results>&count=1"
