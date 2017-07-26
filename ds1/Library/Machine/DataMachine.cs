﻿//
// The MIT License(MIT)
//
// Copyright(c) 2014 Demonsaw LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

namespace DemonSaw.Machine
{
	using DemonSaw.Data;
	using DemonSaw.Utility;
	using System.Threading;

	public abstract class DataMachine : DemonMachine
	{
		// Properties
		private ManualResetEvent Event { get; set; }
		public ItemData Data { get; set; }

		// Constructors
		protected DataMachine() { Event = new ManualResetEvent(false); }
		protected DataMachine(ItemData data) : this() { Data = data; }

		// State Machine	
		protected override void Main()
		{
			Data.Status = DemonStatus.Info;
		}

		protected override void Update()
		{
			base.Update();

			Data.Update();
		}

		// Interface
		public override void Restart()
		{
			base.Restart();

			Data.Status = DemonStatus.Info;
		}

		// Utility
		protected void Lock() { Event.Reset(); }
		protected void Unlock() { Event.Set(); }
		protected void Wait() { Event.WaitOne(); }
	}
}
