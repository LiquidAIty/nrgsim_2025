import torch
from torch import nn
import math
import numpy as np

Path = "c:/project/trading/dagobert/Data/Signals.csv"
Device = "cpu"
NumSignals = 8
Batch = 15
Split = 90
Epochs = 30
Neurons = 256
Rate = 0.001
Models = []

def neural_init():
	global Device
	Device = (
	    "cuda"
	    if torch.cuda.is_available()
	    else "mps"
	    if torch.backends.mps.is_available()
	    else "cpu"
	)
	
def network():	
	model = nn.Sequential(
		nn.Linear(NumSignals,Neurons),
		nn.ReLU(),
		nn.Linear(Neurons,Neurons),
		nn.ReLU(),
		nn.Linear(Neurons,1),
		nn.Sigmoid()
	).to(Device)
	loss = nn.BCELoss()
	optimizer = torch.optim.Adam(model.parameters(),lr=Rate)
	return model,loss,optimizer 

def data(Path,Mode):
	global NumSignals
	Data = np.loadtxt(Path,delimiter=',')
	Size = len(Data)
	NumSignals = len(Data[0,:])-1
	print(f"Size {Size} Num {NumSignals}")
	if Mode == 2: 	# Test
		Start = math.ceil(Size*Split/100)
		End = Size
	elif Mode == 1: # Train
		Start = 0
		End = math.floor(Size*Split/100)
	else:	# Zorro
		Start = 0
		End = Size
	Xy = Data[Start:End,0:NumSignals+1]
	return Xy

def split(Xy):
	X = torch.tensor(Xy[:,0:NumSignals],dtype=torch.float32,device=Device)
	y = torch.tensor(Xy[:,NumSignals].reshape(-1,1),dtype=torch.float32,device=Device)
	y = torch.where(y > 0.,1.,0.)
	return X,y

def train(N,Xy):
	global Models
	model,loss,optimizer = network()
	X,y = split(Xy)
	for j in range(Epochs):
		for i in range(0,len(X),Batch):
			y_pred = model(X[i:i+Batch])
			Loss = loss(y_pred,y[i:i+Batch])
			optimizer.zero_grad()
			Loss.backward()
			optimizer.step()
		print(f"Epoch {j}, Loss {Loss.item():>5f}")
	Models.append(model)
	return Loss.item()*100

def neural_train(N,Path):
	print(f"Train: {Path}")
	Xy = data(Path,0)
	return train(N,Xy)

def neural_predict(N,X):
	if N >= len(Models):
		print(f"Error: Predict {N} >= {len(Models)}")
		return 0
	model = Models[N]
	X = torch.tensor(X,dtype=torch.float32,device=Device)
	with torch.no_grad():
		y_pred = model(X)
	return y_pred

def neural_save(Path):
	torch.save(Models,Path)
	print(f"{len(Models)} models saved")

def neural_load(Path):
	global Models
	Models.clear()
	Models = torch.load(Path)
	for i in range(len(Models)):
		Models[i].eval()
	print(f"{len(Models)} models loaded")

def test(N,Xy):
	model = Models[N]
	model.eval()
	X,y = split(Xy)
	with torch.no_grad():
		y_pred = model(X)
		Accuracy = (y_pred.round() == y).float().mean()
	print(f"Accuracy {Accuracy:>4f}")

def test_signals():
	neural_init()
	Xy_train = data(Path,1)
	train(0,Xy_train)
	Rules = "c:/project/trading/dagobert/data/models.ml"
	neural_save(Rules)
	neural_load(Rules)
	Xy_test = data(Path,2)
	test(0,Xy_test)
	print("Done")

#test_signals()

