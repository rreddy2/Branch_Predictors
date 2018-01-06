
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <bitset>
#include <string>


using namespace std;

/*
	We will have prediction tables of variable size (determine which slot to use by doing Predictor=TableOfPredictors[PC%Size]
	All of the table values will be initialized as '1'
*/
int oneBitBimodal(vector<long> branches, vector<string> actuals, int tableSize)
{
	int TableOfPredictors[tableSize];
	int j;
	int predictor;
	long PC = 0;
	int accuracyCounter = 0;
	
	//initialize the table to be 1
	for(j = 0; j <tableSize; j++)
	{
		TableOfPredictors[j] = 1;
	}
	
	for(j = 0; j < branches.size();j++)
	{
		
		PC = branches[j];
		
		predictor = TableOfPredictors[PC%tableSize];
		
	
		
		if(predictor == 1 && actuals[j] == "T")
		{
			accuracyCounter++;
			
		}
		if(predictor == 1 && actuals[j] == "NT")
		{
			TableOfPredictors[PC%tableSize] = 0;
		}
		if(predictor == 0 && actuals[j] == "NT")
		{
			accuracyCounter++;
		}
		if(predictor == 0 && actuals[j] == "T")
		{
			TableOfPredictors[PC%tableSize] = 1;
		}
			
	}
	
		return accuracyCounter;
}

/* 
	Similar to oneBit but there's two bits in the table
*/
int twoBitBimodal(vector<long>branches, vector<string> actuals, int tableSize)
{
	int TableOfPredictors[tableSize];
	int j = 0;
	int predictor = 0;
	long PC = 0;
	int accuracyCounter = 0;
	
	//initialize the table to be 11
	for(j = 0; j <tableSize; j++)
	{
		TableOfPredictors[j] = 11;
	}
	
	for(j = 0; j < branches.size(); j++)
	{		   
		PC = branches[j];
		
		predictor = TableOfPredictors[PC%tableSize];
		
	
		
		if((predictor == 11) && (actuals[j] == "T"))	//strongly taken and taken
		{
			accuracyCounter++;
		}
		else if((predictor == 11) && (actuals[j] == "NT"))	//strongly taken and not taken
		{
			TableOfPredictors[PC%tableSize] = 10;
		}
		else if((predictor == 10) && (actuals[j] == "T"))	//taken and taken
		{
			accuracyCounter++;
			TableOfPredictors[PC%tableSize] = 11;
		}
		else if((predictor == 10) && (actuals[j] == "NT"))	//taken and not taken
		{
			TableOfPredictors[PC%tableSize] = 01;
		}
		else if((predictor == 01) && (actuals[j] == "T"))	//not taken and taken
		{
			TableOfPredictors[PC%tableSize] = 10;
		}
		else if((predictor == 01) && (actuals[j] == "NT"))	//not taken and not taken
		{
			accuracyCounter++;
			TableOfPredictors[PC%tableSize] = 00;
		}
		else if((predictor == 00) && (actuals[j] == "T"))	//strongly not taken and taken
		{
			TableOfPredictors[PC%tableSize] = 01;
		}
		else if((predictor == 00) && (actuals[j] == "NT"))	//strongly not taken and not taken
		{
			accuracyCounter++;
		}
		else
		{
			cout << predictor << " " << actuals[j] << endl;
		}
	}
	
	return accuracyCounter;
	
}

/*
	create a table with 2048 entries (initialized to be 11) with a global history register of varying length(initialized to be 0)
	XOR the PC and Global History Register
	Check the table for the result's location
	From here do the same as you did for twoBitBimodal
*/
int gShare(vector<long>branches, vector<string> actuals, int globalHistoryLength)
{
	int TableOfPredictors[2048];
	int globalHistory;
	int j;
	int predictor;
	long PC;
	int accuracyCounter = 0;
	int i;
	int taken;
	
	//initialize the table to be 11
	for(j = 0; j < 2048; j++)
	{
		TableOfPredictors[j] = 11;
	}
	
	for(j = 0; j < branches.size();j++)
	{
		
		PC = branches[j];
		
		/*
		
		//Selecting the last n bits of the PC
		if(globalHistoryLength == 3)
		{
			//PC = PC & 0x7;
			PC = PC >> 29;
		}
		if(globalHistoryLength == 4)
		{
			//PC = PC & 0xF
			PC = PC >> 28;
		}
		if(globalHistoryLength == 5)
		{
			//PC = PC & 0x1F;
			PC = PC >> 27;
		}
		if(globalHistoryLength == 6)
		{
			//PC = PC & 0x3F;
			PC = PC >> 26;
		}
		if(globalHistoryLength == 7)
		{
			//PC = PC & 0x7F;
			PC = PC >> 25;
		}
		if(globalHistoryLength == 8)
		{
			//PC = PC & 0xFF;
			PC = PC >> 24;
		}
		if(globalHistoryLength == 9)
		{
			//PC = PC & 0x1FF;
			PC = PC >> 23;
		}
		if(globalHistoryLength == 10)
		{
			//PC = PC & 0x3FF;
			PC = PC >> 22;
		}
		if(globalHistoryLength == 11)
		{
			//PC = PC & 0x7FF;
			PC = PC >> 21;
		}
		*/
	
		
		
		predictor = TableOfPredictors[(PC%2048)^globalHistory];
	
		
		if(predictor == 11 && actuals[j] == "T")	//strongly taken and taken
		{
			accuracyCounter++;
			taken = 1;
		}
		if(predictor == 11 && actuals[j] == "NT")	//strongly taken and not taken
		{
			TableOfPredictors[(PC%2048) ^ globalHistory] = 10;
			taken = 0;
		}
		if(predictor == 10 && actuals[j] == "T")	//taken and taken
		{
			accuracyCounter++;
			TableOfPredictors[(PC%2048) ^ globalHistory] = 11;
			taken = 1;
		}
		if(predictor == 10 && actuals[j] == "NT")	//taken and not taken
		{
			TableOfPredictors[(PC%2048) ^ globalHistory] = 01;
			taken = 0;
		}
		if(predictor == 01 && actuals[j] == "T")	//not taken and taken
		{
			TableOfPredictors[(PC%2048) ^ globalHistory] = 10;
			taken = 1;
		}
		if(predictor == 01 && actuals[j] == "NT")	//not taken and not taken
		{
			accuracyCounter++;
			TableOfPredictors[(PC%2048) ^ globalHistory] = 00;
			taken = 0;
		}
		if(predictor == 00 && actuals[j] == "T")	//strongly not taken and taken
		{
			TableOfPredictors[(PC%2048) ^ globalHistory] = 01;
			taken = 1;
		}
		if(predictor == 00 && actuals[j] == "NT")	//strongly not taken and not taken
		{
			accuracyCounter++;
			taken = 0;
			
		}
	
		globalHistory = globalHistory << 1;
		globalHistory |= taken;
		globalHistory &= (1<<globalHistoryLength)-1;
	}
	
	
		return accuracyCounter;
			
}

/*
	Predict using gshare and bimodal and then see which one is correct and adjust table indexes accordingly.
*/
int tournamentPredictor(vector<long> branches, vector<string> actuals)
{
	int selectorTable[2048];
	int j = 0;
	int TableOfPredictorsGSHARE[2048];
	int globalHistory = 0;
	int predictorGSHARE;
	int i;
	int taken;
	int TableOfPredictorsBIMODAL[2048];
	int predictorBIMODAL;
	int selectIndex = 0;
	long PC;
	
	int accuracyCounter = 0;
	
	
	//initialize the table to be 11
	for(j = 0; j <2048; j++)
	{
		TableOfPredictorsBIMODAL[j] = 11;
	}
	//initialize the table to be 11
	for(j = 0; j < 2048; j++)
	{
		TableOfPredictorsGSHARE[j] = 11;
	}
	//initialize the selectorTable to prefer gshare
	for(j = 0; j < 2048; j++)
	{
		selectorTable[j] = 00;
	}
	
	
	for(j = 0; j < branches.size(); j++)
	{
		PC = branches[j];
		
		predictorGSHARE = TableOfPredictorsGSHARE[(PC%2048)^globalHistory];
		
		predictorBIMODAL = TableOfPredictorsBIMODAL[PC%2048];
		
		selectIndex = selectorTable[PC%2048];
		
		//cout << PC%2048 << "\t" << selectIndex << endl;
		
		
		if(selectIndex == 11)		//prefer bimodal
		{
			//check what bimodal would return and compare it to the actuals
			//if the bimodal is correct, increment accuracyCounter, if not don't
			if((predictorBIMODAL == 11) && (actuals[j] == "T"))	//strongly taken and taken
			{
				accuracyCounter++;
			}
			else if((predictorBIMODAL == 11) && (actuals[j] == "NT"))	//strongly taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "T"))	//taken and taken
			{
				accuracyCounter++;
				TableOfPredictorsBIMODAL[PC%2048] = 11;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "NT"))	//taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "T"))	//not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "NT"))	//not taken and not taken
			{
				accuracyCounter++;
				TableOfPredictorsBIMODAL[PC%2048] = 00;
			}
			else if((predictorBIMODAL == 00) && (actuals[j] == "T"))	//strongly not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			else if((predictorBIMODAL == 00) && (actuals[j] == "NT"))	//strongly not taken and not taken
			{
				accuracyCounter++;
			}
			
			//check what gshare would return
			
			if(predictorGSHARE == 11 && actuals[j] == "T")	//strongly taken and taken
			{
				taken = 1;
			}
			if(predictorGSHARE == 11 && actuals[j] == "NT")	//strongly taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 0;
			}
			if(predictorGSHARE == 10 && actuals[j] == "T")	//taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 11;
				taken = 1;
			}
			if(predictorGSHARE == 10 && actuals[j] == "NT")	//taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 0;
			}
			if(predictorGSHARE == 01 && actuals[j] == "T")	//not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 1;
			}
			if(predictorGSHARE == 01 && actuals[j] == "NT")	//not taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 00;
				taken = 0;
			}
			if(predictorGSHARE == 00 && actuals[j] == "T")	//strongly not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 1;
			}
			if(predictorGSHARE == 00 && actuals[j] == "NT")	//strongly not taken and not taken
			{
				taken = 0;
			}
	
			globalHistory = globalHistory << 1;
			globalHistory |= taken;
			globalHistory &= (1<<11)-1;
			
			//if they're the same then don't change the index, if one's right and the other's wrong
			//change it to favor whoever is right
			
			if((predictorGSHARE == 11 || predictorGSHARE == 10) && (predictorBIMODAL == 01 || predictorBIMODAL == 00) && (actuals[j] == "T"))
			{
				selectorTable[PC%2048] = 10;
			}
			if((predictorGSHARE == 00 || predictorGSHARE == 01) && (predictorBIMODAL == 11 || predictorBIMODAL == 10) && (actuals[j] == "NT"))
			{
				selectorTable[PC%2048] = 10;
			}
		}
		else if(selectIndex == 10)	//weakly prefer bimodal
		{
			//check what bimodal would return and compare it to the actuals
			//if the bimodal is correct, increment accuracyCounter, if not don't
			if((predictorBIMODAL == 11) && (actuals[j] == "T"))	//strongly taken and taken
			{
				accuracyCounter++;
			}
			else if((predictorBIMODAL == 11) && (actuals[j] == "NT"))	//strongly taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "T"))	//taken and taken
			{
				accuracyCounter++;
				TableOfPredictorsBIMODAL[PC%2048] = 11;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "NT"))	//taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "T"))	//not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "NT"))	//not taken and not taken
			{
				accuracyCounter++;
				TableOfPredictorsBIMODAL[PC%2048] = 00;
			}
			else if((predictorBIMODAL == 00) && (actuals[j] == "T"))	//strongly not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			else if((predictorBIMODAL == 00) && (actuals[j] == "NT"))	//strongly not taken and not taken
			{
				accuracyCounter++;
			}
			
			//check what gshare would return
			
			if(predictorGSHARE == 11 && actuals[j] == "T")	//strongly taken and taken
			{
				taken = 1;
			}
			if(predictorGSHARE == 11 && actuals[j] == "NT")	//strongly taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 0;
			}
			if(predictorGSHARE == 10 && actuals[j] == "T")	//taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 11;
				taken = 1;
			}
			if(predictorGSHARE == 10 && actuals[j] == "NT")	//taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 0;
			}
			if(predictorGSHARE == 01 && actuals[j] == "T")	//not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 1;
			}
			if(predictorGSHARE == 01 && actuals[j] == "NT")	//not taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 00;
				taken = 0;
			}
			if(predictorGSHARE == 00 && actuals[j] == "T")	//strongly not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 1;
			}
			if(predictorGSHARE == 00 && actuals[j] == "NT")	//strongly not taken and not taken
			{
				taken = 0;
			}
	
			globalHistory = globalHistory << 1;
			globalHistory |= taken;
			globalHistory &= (1<<11)-1;
			
			//if they're the same then don't change the index, if one's right and the other's wrong
			//change it to favor whoever is right
			
			if((predictorGSHARE == 11 || predictorGSHARE == 10) && (predictorBIMODAL == 01 || predictorBIMODAL == 00) && (actuals[j] == "T"))
			{
				selectorTable[PC%2048] = 01;
			}
			if((predictorGSHARE == 00 || predictorGSHARE == 01) && (predictorBIMODAL == 11 || predictorBIMODAL == 10) && (actuals[j] == "T"))
			{
				selectorTable[PC%2048] = 11;
			}
			if((predictorGSHARE == 11 || predictorGSHARE == 10) && (predictorBIMODAL == 01 || predictorBIMODAL == 00) && (actuals[j] == "NT"))
			{
				selectorTable[PC%2048] = 11;
			}
			if((predictorGSHARE == 00 || predictorGSHARE == 01) && (predictorBIMODAL == 10 || predictorBIMODAL == 11) && (actuals[j] == "NT"))
			{
				selectorTable[PC%2048] = 01;
			}
		}
		else if(selectIndex == 01)	//weakly prefer gshare
		{
			//check what Gshare would return and compare it to the actuals
			//if the gshare is correct, increment accuracyCounter, if not don't
			
			if(predictorGSHARE == 11 && actuals[j] == "T")	//strongly taken and taken
			{
				accuracyCounter++;
				taken = 1;
			}
			if(predictorGSHARE == 11 && actuals[j] == "NT")	//strongly taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 0;
			}
			if(predictorGSHARE == 10 && actuals[j] == "T")	//taken and taken
			{
				accuracyCounter++;
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 11;
				taken = 1;
			}
			if(predictorGSHARE == 10 && actuals[j] == "NT")	//taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 0;
			}
			if(predictorGSHARE == 01 && actuals[j] == "T")	//not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 1;
			}
			if(predictorGSHARE == 01 && actuals[j] == "NT")	//not taken and not taken
			{
				accuracyCounter++;
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 00;
				taken = 0;
			}
			if(predictorGSHARE == 00 && actuals[j] == "T")	//strongly not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 1;
			}
			if(predictorGSHARE == 00 && actuals[j] == "NT")	//strongly not taken and not taken
			{
				accuracyCounter++;
				taken = 0;
			}
	
			globalHistory = globalHistory << 1;
			globalHistory |= taken;
			globalHistory &= (1<<11)-1;
			
			//check BIMODAL now
			
			if((predictorBIMODAL == 11) && (actuals[j] == "NT"))	//strongly taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "T"))	//taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 11;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "NT"))	//taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "T"))	//not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "NT"))	//not taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 00;
			}
			else if((predictorBIMODAL == 00) && (actuals[j] == "T"))	//strongly not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			
			//if they're the same then don't change the index, if one's right and the other's wrong
			//change it to favor whoever is right
			
			if((predictorGSHARE == 11 || predictorGSHARE == 10) && (predictorBIMODAL == 01 || predictorBIMODAL == 00) && (actuals[j] == "T"))
			{
				selectorTable[PC%2048] = 00;
			}
			if((predictorGSHARE == 00 || predictorGSHARE == 01) && (predictorBIMODAL == 11 || predictorBIMODAL == 10) && (actuals[j] == "T"))
			{
				selectorTable[PC%2048] = 10;
			}
			if((predictorGSHARE == 11 || predictorGSHARE == 10) && (predictorBIMODAL == 01 || predictorBIMODAL == 00) && (actuals[j] == "NT"))
			{
				selectorTable[PC%2048] = 10;
			}
			if((predictorGSHARE == 00 || predictorGSHARE == 01) && (predictorBIMODAL == 10 || predictorBIMODAL == 11) && (actuals[j] == "NT"))
			{
				selectorTable[PC%2048] = 00;
			}
			
		}
		else if(selectIndex == 00)	//prefer gshare
		{
			//check what Gshare would return and compare it to the actuals
			//if the gshare is correct, increment accuracyCounter, if not don't
			
			if(predictorGSHARE == 11 && actuals[j] == "T")	//strongly taken and taken
			{
				accuracyCounter++;
				taken = 1;
			}
			if(predictorGSHARE == 11 && actuals[j] == "NT")	//strongly taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 0;
			}
			if(predictorGSHARE == 10 && actuals[j] == "T")	//taken and taken
			{
				accuracyCounter++;
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 11;
				taken = 1;
			}
			if(predictorGSHARE == 10 && actuals[j] == "NT")	//taken and not taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 0;
			}
			if(predictorGSHARE == 01 && actuals[j] == "T")	//not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 10;
				taken = 1;
			}
			if(predictorGSHARE == 01 && actuals[j] == "NT")	//not taken and not taken
			{
				accuracyCounter++;
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 00;
				taken = 0;
			}
			if(predictorGSHARE == 00 && actuals[j] == "T")	//strongly not taken and taken
			{
				TableOfPredictorsGSHARE[(PC%2048) ^ globalHistory] = 01;
				taken = 1;
			}
			if(predictorGSHARE == 00 && actuals[j] == "NT")	//strongly not taken and not taken
			{
				accuracyCounter++;
				taken = 0;
			}
	
			globalHistory = globalHistory << 1;
			globalHistory |= taken;
			globalHistory &= (1<<11)-1;
			
			//check BIMODAL now
			
			if((predictorBIMODAL == 11) && (actuals[j] == "NT"))	//strongly taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "T"))	//taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 11;
			}
			else if((predictorBIMODAL == 10) && (actuals[j] == "NT"))	//taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "T"))	//not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 10;
			}
			else if((predictorBIMODAL == 01) && (actuals[j] == "NT"))	//not taken and not taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 00;
			}
			else if((predictorBIMODAL == 00) && (actuals[j] == "T"))	//strongly not taken and taken
			{
				TableOfPredictorsBIMODAL[PC%2048] = 01;
			}
			
			//if they're the same then don't change the index, if one's right and the other's wrong
			//change it to favor whoever is right
			
			if((predictorGSHARE == 00 || predictorGSHARE == 01) && (predictorBIMODAL == 11 || predictorBIMODAL == 10) && (actuals[j] == "T"))
			{
				//cout << "GO BIMODAL" << endl;
				selectorTable[PC%2048] = 01;
			}
			if((predictorGSHARE == 11 || predictorGSHARE == 10) && (predictorBIMODAL == 01 || predictorBIMODAL == 00) && (actuals[j] == "NT"))
			{
				//cout << "GO BIMODAL" <<endl;
				selectorTable[PC%2048] = 01;
			}
		}
		
		
	}
	
	
	return accuracyCounter;
}

int main(int argc, char **argv)
{
	ifstream file;
	ofstream outputFile;
	int i = 0;
	string line;
	
	int alwaysTakenPredictorCounter = 0;
	int alwaysNotTakenPredictorCounter = 0;
	int singleBit16Counter = 0;
	int singleBit32Counter = 0;
	int singleBit128Counter = 0;
	int singleBit256Counter = 0;
	int singleBit512Counter = 0;
	int singleBit1024Counter = 0;
	int singleBit2048Counter = 0;
	int twoBit16Counter = 0;
	int twoBit32Counter = 0;
	int twoBit128Counter = 0;
	int twoBit256Counter = 0;
	int twoBit512Counter = 0;
	int twoBit1024Counter = 0;
	int twoBit2048Counter = 0;
	int gShare3Bit = 0;
	int gShare4Bit = 0;
	int gShare5Bit = 0;
	int gShare6Bit = 0;
	int gShare7Bit = 0;
	int gShare8Bit = 0;
	int gShare9Bit = 0;
	int gShare10Bit = 0;
	int gShare11Bit = 0;
	int tournamentPredictorBit = 0;

	
	
	vector<long> branches;
	string temp;
	long convertedString;
	vector<string> actuals;
	
	file.open(argv[1]);
	
	if(file.is_open())
	{
		//the first 10 characters are the branch address
		//the next character is a space
		//the next character (or two) is NT/T
		while(getline(file,line))
		{
			stringstream ss;
			ss << hex << line.substr(0,10);
			ss >> convertedString;
	
			branches.push_back(convertedString);
			
			if(line[11] == 'N')
			{
				actuals.push_back(line.substr(11,2));
			
			}
			else
			{
			
				actuals.push_back(line.substr(11,1));
			}
		}
	}

	
	//check how many Ts and NTs there are in the branches
	for(i = 0; i < actuals.size(); i++)
	{
		if(actuals[i] == "T")
		{
			alwaysTakenPredictorCounter++;
		}
		else if(actuals[i] == "NT")
		{
			alwaysNotTakenPredictorCounter++;
		}
		else
		{
			cout << "Has to be T or NT" << endl;
			exit(1);
		}
	}
	
	cout << "Before Testing" << endl;
	//TESTING SINGLE BIT COUNTER
	singleBit16Counter = oneBitBimodal(branches, actuals, 16);
	singleBit32Counter = oneBitBimodal(branches, actuals, 32);
	singleBit128Counter = oneBitBimodal(branches, actuals, 128);
	singleBit256Counter = oneBitBimodal(branches,actuals, 256);
	singleBit512Counter = oneBitBimodal(branches,actuals, 512);
	singleBit1024Counter = oneBitBimodal(branches,actuals,1024);
	singleBit2048Counter = oneBitBimodal(branches, actuals, 2048);
	
	cout << "Before 2bit" << endl;
	//TESTING TWO BIT COUNTER
	twoBit16Counter = twoBitBimodal(branches, actuals, 16);
	twoBit32Counter = twoBitBimodal(branches, actuals, 32);
	twoBit128Counter = twoBitBimodal(branches, actuals, 128);
	twoBit256Counter = twoBitBimodal(branches,actuals, 256);
	twoBit512Counter = twoBitBimodal(branches,actuals, 512);
	twoBit1024Counter = twoBitBimodal(branches,actuals,1024);
	twoBit2048Counter = twoBitBimodal(branches, actuals, 2048);
	cout << "Before GSHARE" << endl;
	
	//TESTING GSHARE
	gShare3Bit = gShare(branches, actuals, 3);
	gShare4Bit = gShare(branches, actuals, 4);
	gShare5Bit = gShare(branches, actuals, 5);
	gShare6Bit = gShare(branches, actuals, 6);
	gShare7Bit = gShare(branches, actuals, 7);
	gShare8Bit = gShare(branches, actuals, 8);
	gShare9Bit = gShare(branches, actuals, 9);
	gShare10Bit = gShare(branches, actuals, 10);
	gShare11Bit = gShare(branches, actuals, 11);
	
	cout << "Before Tournament" << endl;
	//TESTING TOURNAMENT
	tournamentPredictorBit = tournamentPredictor(branches, actuals);
	
	cout << "after testing" << endl;
	outputFile.open(argv[2]);
	
	
	//PRINTING EVERYTHING
	if(outputFile.is_open())
	{
		outputFile << alwaysTakenPredictorCounter << "," << branches.size() << ";" << endl;
		outputFile << alwaysNotTakenPredictorCounter << "," << branches.size() << ";" << endl;
	
		outputFile << singleBit16Counter << "," << branches.size() << ";" << " " << singleBit32Counter << "," << branches.size() << ";" << " ";
		outputFile << singleBit128Counter << "," << branches.size() << ";" << " " << singleBit256Counter << "," << branches.size() << ";" << " ";
		outputFile << singleBit512Counter << "," << branches.size() << ";" << " " << singleBit1024Counter << "," << branches.size() << ";" << " ";
		outputFile << singleBit2048Counter << "," << branches.size() << ";" << endl;
	
		outputFile << twoBit16Counter << "," << branches.size() << ";" << " " << twoBit32Counter << "," << branches.size() << ";" << " ";
		outputFile << twoBit128Counter << "," << branches.size() << ";" << " " << twoBit256Counter << "," << branches.size() << ";" << " ";
		outputFile << twoBit512Counter << "," << branches.size() << ";" << " " << twoBit1024Counter << "," << branches.size() << ";" << " ";
		outputFile << twoBit2048Counter << "," << branches.size() << ";" << endl;
	
		outputFile << gShare3Bit << "," << branches.size() << ";" << " " << gShare4Bit << "," << branches.size() << ";" << " ";
		outputFile << gShare5Bit << "," << branches.size() << ";" << " " << gShare6Bit << "," << branches.size() << ";" << " ";
		outputFile << gShare7Bit << "," << branches.size() << ";" << " " << gShare8Bit << "," << branches.size() << ";" << " ";
		outputFile << gShare9Bit << "," << branches.size() << ";" << " " << gShare10Bit << "," << branches.size() << ";" << " ";
		outputFile << gShare11Bit << "," << branches.size() << ";" << endl;
	
		outputFile << tournamentPredictorBit << "," << branches.size() << ";" << endl;
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
	outputFile.close();
	
	branches.clear();
	actuals.clear();
	branches.shrink_to_fit();
	actuals.shrink_to_fit();
	
	return 0;
}