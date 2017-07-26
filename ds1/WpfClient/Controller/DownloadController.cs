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

namespace DemonSaw.Controller
{
	using DemonSaw.Component;
	using DemonSaw.Entity;
	using System;
	using System.Diagnostics;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Threading;

	// TODO
	//
	// [CLIENT ENTITY]
	// * DownloadQueueComponent
	//
	public sealed class DownloadController : EntityController
	{
		#region Properties
		private static LogController LogController { get { return Global.LogController; } }
		private static MenuController MenuController { get { return Global.MenuController; } }
		#endregion

		#region Constructors
		public DownloadController(DataGrid view, TextBlock title) : base(view, title) { }
		#endregion

		#region Interface
		public override void Update()
		{
			base.Update();

			if (Application.Current != null)
			{
				Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(delegate
				{
					Title.Text = ((List.Count != 1) ? string.Format("{0:n0} Files", List.Count) : "1 File");
				}));
			}
		}
		#endregion

		#region Commands
		public void Download(BrowseComponent browse)
		{
			Debug.Assert(browse != null);

			Entity entity = new Entity();
			EntityIdleComponent idle = new EntityIdleComponent();
			entity.Add(idle);

			EntityStoreComponent store = new EntityStoreComponent(browse.Owner);
			entity.Add(store);

			FileComponent file = new FileComponent(browse);
			entity.Add(file);

			TransferComponent transfer = new TransferComponent();
			entity.Add(transfer);

			DownloadMachineComponent machine = new DownloadMachineComponent(browse.ClientId);
			entity.Add<MachineComponent>(machine);
#if DEBUG
			LogComponent log = new LogComponent(LogController);
			entity.Add(log);
#endif
			Add(entity);
		}

		public void Download(FileComponent comp)
		{
			Debug.Assert(comp != null);

			Entity entity = new Entity();
			EntityIdleComponent idle = new EntityIdleComponent();
			entity.Add(idle);

			EntityStoreComponent store = new EntityStoreComponent(comp.Owner);
			entity.Add(store);

			FileComponent file = new FileComponent(comp);
			entity.Add(file);

			TransferComponent transfer = new TransferComponent();
			entity.Add(transfer);

			DownloadMachineComponent machine = new DownloadMachineComponent();
			entity.Add<MachineComponent>(machine);
#if DEBUG
			LogComponent log = new LogComponent(LogController);
			entity.Add(log);
#endif
			Add(entity);
		}
		#endregion
	}
}
